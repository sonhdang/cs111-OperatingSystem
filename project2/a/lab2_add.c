//NAME: Son Dang
//EMAIL: sonhdang@ucla.edu
//ID: 105215636

#include <getopt.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long long counter = 0;
long lock = 0;
int opt_yield = 0;
char opt_sync = EOF;
int num_threads = 1;         // Number of threads
int num_iterations = 1;      // Number of iterations
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void add(long long *pointer, long long value)
{
    long long prev, sum;

    switch(opt_sync)        //SYNCHRONIZATION
    {
        case 'm':           //MUTEX
            pthread_mutex_lock(&mutex);
            sum = *pointer + value;
            if(opt_yield)
                sched_yield();
            *pointer = sum;
            pthread_mutex_unlock(&mutex);
            break;
        case 's':           //SPINLOCK
            while(__sync_lock_test_and_set(&lock, 1));
            sum = *pointer + value;
            if(opt_yield)
                sched_yield();
            *pointer = sum;
            __sync_lock_release(&lock);
            break;
        case 'c':           //COMPARE AND SWAP
            do 
            {
                prev = *pointer;
                sum = prev + value;
            } while (__sync_bool_compare_and_swap(pointer, prev, sum) == 0);
            break;
        default:            // NO SYNCHRONIZATION
            sum = *pointer + value;
            if(opt_yield)
                sched_yield();
            *pointer = sum;
    }
}

void* add_thread(void* arg)
{
    int *iter = (int *) arg;
    for (int i = 0; i < *iter; i++)
    {
        add(&counter, 1);
        add(&counter, -1);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    struct timespec start_time;
    struct timespec end_time;
    char test[20] = "add";
    int c;
    
    while(1)
    {
        static struct option long_options[] =
        {
            {"threads", optional_argument, NULL, 't'},
            {"iterations", optional_argument, NULL, 'i'},
            {"yield", no_argument, NULL, 'y'},
            {"sync", required_argument, NULL, 's'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        int sscanf_ret = -1;

        if (c == -1)
            break;
        
        switch (c)
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
                strcat(test, "-yield");
                opt_yield = 1;
                break;
            case 's':   //Synchronization
                if (strlen(optarg) == 1 && 
                    (optarg[0] == 'm' || optarg[0] == 'c' || optarg[0] == 's'))
                {
                    sscanf_ret = sscanf(optarg, "%c", &opt_sync);
                }
                else
                {
                    printf("argument is either 'm', 's', or 'c'\n");
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }

    pthread_t threads[num_threads];
    int thread_ret;
    int clock_ret = clock_gettime(CLOCK_MONOTONIC, &start_time);    //START TIME

    for (int i = 0; i <= num_threads; i++)      //Creating specified number of threads
    {
        thread_ret = pthread_create(&threads[i], NULL, add_thread, (void *) &num_iterations);
        if (thread_ret)
        {
            printf("ERROR: pthread_create() returns %d\n", thread_ret);
            exit(-1);
        }
    }

    for (int i = 0; i <= num_threads; i++)      //Waiting for all threads to join
    {
        thread_ret = pthread_join(threads[i], NULL);
        if (thread_ret)
        {
            printf("ERROR: pthread_join() returns %d\n", thread_ret);
            exit(-1);
        }
    }

    switch(opt_sync)
    {
        case 'm':
            strcat(test, "-m");
            break;
        case 's':
            strcat(test, "-s");
            break;
        case 'c':
            strcat(test, "-c");
            break;
        default:
            strcat(test, "-none");
    }

    clock_ret = clock_gettime(CLOCK_MONOTONIC, &end_time);          //END TIME

    long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
    long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
    long long num_operations = num_threads * num_iterations * 2;
    long long runtime = end_ns -  start_ns;
    int average_time = runtime / num_operations;

    printf("%s,%d,%d,%lld,%lld,%d,%lld\n",test, num_threads, num_iterations, 
            num_operations, runtime, average_time, counter);

    if (opt_sync == 'm')
        pthread_mutex_destroy(&mutex);

    exit(EXIT_SUCCESS);
}