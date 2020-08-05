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

#define KEYLENGTH 10

SortedList_t *listhead;
SortedListElement_t *pool;
long lock = 0;
char opt_sync = EOF;
int num_threads = 1;         // Number of threads
int num_iterations = 1;      // Number of iterations
int num_lists = 1;           // Number of lists
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void handler(int num)
{
    fprintf(stderr, "Segmentation fault - %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void* thread_worker(void* arg)      // Thread Function
{
    switch(opt_sync)                // Synchronization options
    {
        case 'm':   //Mutex lock
            pthread_mutex_lock(&mutex);
            break;
        case 's':   //Spin lock
            while(__sync_lock_test_and_set(&lock,1));
            break;
    }

    /* CRITICAL SECTION */
    int startIndex = *((int *)arg) * num_iterations;
    for (int i = startIndex; i < startIndex + num_iterations; i++)
        SortedList_insert(listhead, &pool[i]);

    SortedList_length(listhead);
    for(int i = startIndex; i < startIndex + num_iterations; i++)
    {
        SortedListElement_t *e = SortedList_lookup(listhead, pool[i].key);
        SortedList_delete(e);
    }

    switch(opt_sync)
    {
        case 'm':   //Mutex unlock 
            pthread_mutex_unlock(&mutex);
            break;
        case 's':   //Spin unlock
            __sync_lock_release(&lock);
            break;
    }
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
    listhead = malloc(sizeof(*listhead));
    
    while(1)    //Getting the options from stdin
    {
        static struct option long_options[] =
        {
            {"threads", optional_argument, NULL, 't'},
            {"iterations", optional_argument, NULL, 'i'},
            {"yield", required_argument, NULL, 'y'},
            {"sync", required_argument, NULL, 's'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        int sscanf_ret = -1;

        if (c == -1)
            break;

        switch(c)
        {
            case 't':   // THREAD
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
            case 's':   //Synchronization
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
        }
    }

    pthread_t threads[num_threads];
    int thread_id[num_threads];
    int thread_ret;
    
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

    printf("%s,%d,%d,%d,%lld,%lld,%d\n",name, num_threads, num_iterations, 
            num_lists, num_operations, runtime, average_time);

    free(pool);

    
   exit(EXIT_SUCCESS);
}