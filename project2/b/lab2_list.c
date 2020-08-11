//NAME: Son Dang
//EMAIL: sonhdang@ucla.edu
//ID: 105215636

#include "SortedList.h"
#include <getopt.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define KEYLENGTH 20

SortedList_t *listheads;
SortedListElement_t *pool;
int* locks;
char opt_sync = EOF;
int num_threads = 1;         // Number of threads
int num_iterations = 1;      // Number of iterations
int num_lists = 1;           // Number of lists
long long wait_time = 0;     // Time waiting for the lock
pthread_mutex_t *mutexes;

void handler(int num)
{
    fprintf(stderr, "Segmentation fault - %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


int hashkey(const char* key)
{
    unsigned int hash = 7;
    for (int i = 0; i < KEYLENGTH; i++)
    {
        hash = hash * 31 + key[i];
    }

    return hash % num_lists;
}

void* thread_worker(void* arg)      // Thread Function
{
    struct timespec start_time;
    struct timespec end_time;
    int startIndex = *((int *)arg) * num_iterations;

    for (int i = startIndex; i < startIndex + num_iterations; i++)
    {
        /*SORTEDLIST_INSERT*/
        unsigned int list_num = hashkey(pool[i].key);
        clock_gettime(CLOCK_MONOTONIC, &start_time);    //START TIME
        switch(opt_sync)                // Synchronization options
        {
            case 'm':   //Mutex lock
                pthread_mutex_lock(&mutexes[list_num]);
                break;
            case 's':   //Spin lock
                while(__sync_lock_test_and_set(&locks[list_num],1));
                break;
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);    //END TIME

        long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
        long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
        wait_time += end_ns - start_ns;

        SortedList_insert(&listheads[list_num], &pool[i]);
        

        switch(opt_sync)
        {
            case 'm':   //Mutex unlock 
                pthread_mutex_unlock(&mutexes[list_num]);
                break;
            case 's':   //Spin unlock
                __sync_lock_release(&locks[list_num]);
                break;
        }
    }

    /*SORTEDLIST_LENGTH*/
    unsigned long total_length = 0;
    for (int i = 0; i < num_lists; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);    //START TIME
        switch(opt_sync)                // Synchronization options
        {
            case 'm':   //Mutex lock
                pthread_mutex_lock(&mutexes[i]);
                break;
            case 's':   //Spin lock
                while(__sync_lock_test_and_set(&locks[i],1));
                break;
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);    //END TIME

        long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
        long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
        wait_time += end_ns - start_ns;
    }

    for(int i = 0; i < num_lists; i++)
    {
        total_length += SortedList_length(&listheads[i]);
    }

    for (int i = 0; i < num_lists; i++)
    {
        switch(opt_sync)
        {
            case 'm':   //Mutex unlock 
                pthread_mutex_unlock(&mutexes[i]);
                break;
            case 's':   //Spin unlock
                __sync_lock_release(&locks[i]);
                break;
        }
    }

    
    
    /*SORTEDLIST_LOOKUP AND SORTEDLIST_DELETE*/
    char* temp_key = (char*) malloc(sizeof(char)*KEYLENGTH);
    for(int i = startIndex; i < startIndex + num_iterations; i++)
    {
        unsigned int list_num = hashkey(pool[i].key);
        strcpy(temp_key, pool[i].key);
        clock_gettime(CLOCK_MONOTONIC, &start_time);    //START TIME
        switch(opt_sync)                // Synchronization options
        {
            case 'm':   //Mutex lock
                pthread_mutex_lock(&mutexes[list_num]);
                break;
            case 's':   //Spin lock
                while(__sync_lock_test_and_set(&locks[list_num],1));
                break;
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);    //END TIME

        long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
        long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
        wait_time += end_ns - start_ns;

        SortedListElement_t *e = SortedList_lookup(&listheads[list_num], temp_key);
        SortedList_delete(e);

        switch(opt_sync)
        {
            case 'm':   //Mutex unlock 
                pthread_mutex_unlock(&mutexes[list_num]);
                break;
            case 's':   //Spin unlock
                __sync_lock_release(&locks[list_num]);
                break;
        }
    }
    free(temp_key);

    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, handler);
    //Initializing time
    struct timespec start_time;
    struct timespec end_time;
    //Name of test
    char name[20] = "list";
    //Others
    int c;
    
    
    while(1)    //Getting the options from stdin
    {
        static struct option long_options[] =
        {
            {"threads", optional_argument, NULL, 't'},
            {"iterations", optional_argument, NULL, 'i'},
            {"yield", required_argument, NULL, 'y'},
            {"sync", required_argument, NULL, 's'},
            {"lists", optional_argument, NULL, 'l'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        int sscanf_ret = -1;

        if (c == -1)
            break;

        switch(c)
        {
            case 't':   //THREAD
                sscanf_ret = sscanf(optarg, "%d", &num_threads);
                if( sscanf_ret == EOF)
                {
                    printf("error, NOTHING WAS INPUT");
                } 
                else if (sscanf_ret == 0)
                {
                    printf("input needs to be an integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':   //ITERATION
                sscanf_ret = sscanf(optarg, "%d", &num_iterations);
                if( sscanf_ret == EOF)
                {
                    printf("error, NOTHING WAS INPUT");
                } 
                else if (sscanf_ret == 0)
                {
                    printf("input needs to be an integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'y':   //YIELD
                for (int i = 0; i < (int) strlen(optarg); i++)
                {
                    if (optarg[i] == 'i')
                        opt_yield |= INSERT_YIELD;
                    else if (optarg[i] == 'd')
                        opt_yield |= DELETE_YIELD;
                    else if (optarg[i] == 'l')
                        opt_yield |= LOOKUP_YIELD;
                }
                break;
            case 's':   //SYNCHRONIZATION
                if (strlen(optarg) == 1 && 
                    (optarg[0] == 'm' || optarg[0] == 's'))
                {
                    sscanf_ret = sscanf(optarg, "%c", &opt_sync);
                }
                else
                {
                    printf("argument is either 'm' or 's'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':   //LIST
                sscanf_ret = sscanf(optarg, "%d", &num_lists);
                if( sscanf_ret == EOF)
                {
                    printf("error, NOTHING WAS INPUT");
                } 
                else if (sscanf_ret == 0)
                {
                    printf("input needs to be an integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }

    pthread_t threads[num_threads];
    int thread_id[num_threads];
    int thread_ret;
    listheads = malloc(sizeof(*listheads) * num_lists);
    mutexes = malloc(sizeof(*mutexes) * num_lists);
    if (mutexes == NULL)
    {
        fprintf(stderr, "Memory allocation for mutexes failed - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_lists; i++)
    {
        pthread_mutex_init(&mutexes[i], NULL);
    }

    locks = malloc(sizeof(*locks) * num_lists);
    if (locks == NULL)
    {
        fprintf(stderr, "Memory allocation for locks failed - %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_lists; i++)
    {
        locks[i] = 0;
    }

    
    srand(time(NULL));      // Setting seed for generating random keys
    long long num_elements = num_threads * num_iterations;
    pool = malloc(num_elements * sizeof(*pool));
    for (int i = 0; i < num_elements; i++)      // Initializing the elements with random keys
    {
        char* temp_key = (char*) malloc(sizeof(char)*KEYLENGTH);
        int j;
        for(j = 0; j < KEYLENGTH - 1; j++)
            temp_key[j] = rand() % 26 + 'a';
        temp_key[KEYLENGTH - 1] = '\0';
        pool[i].key = temp_key;
    }

    int clock_ret = clock_gettime(CLOCK_MONOTONIC, &start_time);    //START TIME

    for(int i = 0; i < num_threads; i++)    //Creating specified number of threads
    {
        thread_id[i] = i;
        thread_ret = pthread_create(&threads[i], NULL, thread_worker, (void *) &thread_id[i]);
        if (thread_ret)
        {
            printf("ERROR: pthread_create() returns %d\n", thread_ret);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; i++)      //Waiting for all threads to join
    {
        thread_ret = pthread_join(threads[i], NULL);
        if (thread_ret)
        {
            printf("ERROR: pthread_join() returns %d\n", thread_ret);
            exit(EXIT_FAILURE);
        }
    }

    clock_ret = clock_gettime(CLOCK_MONOTONIC, &end_time);          //END TIME

    switch(opt_yield)   //Naming Yield
    {
        case 1:
            strcat(name, "-i");
            break;
        case 2:
            strcat(name, "-d");
            break;
        case 3:
            strcat(name, "-id");
            break;
        case 4:
            strcat(name, "-l");
            break;
        case 5:
            strcat(name, "-il");
            break;
        case 6:
            strcat(name, "-dl");
            break;
        case 7:
            strcat(name, "-idl");
            break;
        default:
            strcat(name, "-none");
    }

     switch(opt_sync)   //Naming synchronization
    {
        case 's':
            strcat(name, "-s");
            break;
        case 'm':
            strcat(name, "-m");
            break;
        default:
            strcat(name, "-none");
    }

    long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
    long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
    long long num_operations = num_threads * num_iterations * 3;
    long long runtime = end_ns -  start_ns;
    int average_time = runtime / num_operations;
    int average_wait_for_lock = wait_time / num_operations;

    printf("%s,%d,%d,%d,%lld,%lld,%d,%d\n",name, num_threads, num_iterations, 
            num_lists, num_operations, runtime, average_time, average_wait_for_lock);

    for (int i = 0; i < num_lists; i++)
    {
        pthread_mutex_destroy(&mutexes[i]);
    }
    free(pool);
    free(listheads);
    free(mutexes);
    free(locks);

    
   exit(EXIT_SUCCESS);
}