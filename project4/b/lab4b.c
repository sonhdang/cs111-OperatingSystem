#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <sys/types.h>
#include <mraa.h>

// Flag indicates running state of the program
sig_atomic_t volatile run_flag = 1;
sig_atomic_t volatile log_flag = 0;
// Keeping track of Elapsed time
struct timespec start_time;
struct timespec end_time;
// Sensor and button
mraa_aio_context temperature_sensor;
mraa_gpio_context button;
mraa_result_t status;
// Others
int file;

void get_current_time(char** str)
{
    struct timespec real_time;
    struct tm *time;
    clock_gettime(CLOCK_REALTIME, &real_time);
    time = localtime(&(real_time.tv_sec));
    sprintf(*str, "%02d:%02d:%02d", time->tm_hour,
            time->tm_min, time->tm_sec);
}

void button_pressed()
{
    char *output_string = malloc(sizeof(char) * 20);
    char *time_string = malloc(sizeof(char) * 10);
    memset(output_string, 0, 20);
    memset(time_string, 0, 10);
    get_current_time(&time_string);
    sprintf(output_string, "%s SHUTDOWN\n", time_string);
    status = mraa_aio_close(temperature_sensor);
    if (status != MRAA_SUCCESS) {
        exit(EXIT_FAILURE);
    }
    mraa_gpio_close(button);
    if(log_flag == 1)
        write(file, output_string, 20);
    printf(output_string);
    close(file);
    kill(getpid(),SIGINT);
}

int main(int argc, char* argv[])
{
    /* DECLARING VARIABLES */
    char scale = 'F';   // Default scale set to Fahrenheit (F)
    int period = 1;     // Default sampling interval 1 read/sec
    char filename[20];

    /* READING OPTION FROM THE TERMINAL */
    int c;

    while(1)
    {
        static struct option long_options[] =
        {
            {"period", optional_argument, NULL, 'p'},
            {"scale", optional_argument, NULL, 's'},
            {"log", required_argument, NULL, 'l'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        if (c == -1)
            break;
        
        switch (c)
        {
            case 'p':       // Sampling Rate
                if (sscanf(optarg, "%d", &period) == 0)
                {
                    fprintf(stderr, "Argument needs to be an integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':       // Scale
                if (strlen(optarg) == 1 && (optarg[0] == 'C' || optarg[0] == 'F'))
                {
                    scale = optarg[0];
                }
                else
                {
                    fprintf(stderr, "Argument should either be 'C' or 'F'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':       // Log file
                strcpy(filename, optarg);
                log_flag = 1;
                break;
        }
    }

    /* MAIN PROGRAM */
    if (log_flag == 1)
    {
        if (filename == NULL)
        {
            fprintf(stderr, "Error with filename\n");
            exit(1);
        }
        else
        {
            file = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
        }
    }
    
    
    /* BUTTON */

    button = mraa_gpio_init(73);
    mraa_gpio_dir(button, MRAA_GPIO_IN);
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &button_pressed, NULL);


    /* TEMPERATURE SENSOR */
    int value = 0;
    float float_value = 0;
    const int R0 = 100000;
    const int B = 4275;
    float celsius = 0;
    float fahrenheit = 0;
    
    temperature_sensor = mraa_aio_init(0);
    if (temperature_sensor == NULL) {
        fprintf(stderr, "Failed to initialize AIO\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    int fd = 0;
    char buf[100];
    char command[100];
    memset(command, 0, 100);    //initializing the string 'command'
    memset(buf, 0, 100);
    char* pointer;              //points to the position of '\n' in the string
    int pret;
    int timeout;
    struct pollfd *pollfds = malloc(sizeof(*poll));
    while(1)
    {
        strcat(command, buf);   //concatenating input after each read
        memset(buf, 0, 100);    //clearing buffer after each read;
        pointer = strpbrk(command, "\n");
        pollfds->fd = fd;
        pollfds->events = 0;
        pollfds->events |= POLLIN;
        timeout = period * 1000;
        pret = poll(pollfds, 1, timeout);
        if(pret == 0)
        {
            while (pointer != NULL)
            {
                int command_length = pointer - command + 1;
                char temp[100];
                memset(temp, 0, 100); //initializing string 'temp'
                strncpy(temp, command, command_length - 1); //getting the command
                if (command_length < 3)
                {
                    fprintf(stderr, "Command invalid\n");
                }
                else if(command_length > 6)
                {
                    char check[8];
                    memset(check, 0, 8);
                    memcpy(check, &temp[0], 7);
                    check[7] = '\0';
                    if (strcmp(check, "PERIOD=") == 0)
                    {
                        sscanf(temp + 7, "%d", &period);
                        if(log_flag == 1)
                            write(file, command, command_length);
                    }
                    memcpy(check, &temp[0], 6);
                    check[6] = '\0';
                    if(strcmp(check, "SCALE=") == 0)
                    {
                        if (temp[6] == 'C' || temp[6] == 'F')
                        {
                            scale = temp[6];
                            if(log_flag == 1)
                                write(file, command, command_length);
                        }
                        else
                        {
                            fprintf(stderr, "%c is not a valid scale\n", temp[7]);
                        }
                    }
                }
                else if(command_length == 6 || strcmp(temp, "START") == 0)
                {
                    run_flag = 1;
                    if(log_flag == 1)
                        write(file, command, command_length);
                }
                else if(command_length == 5 || strcmp(temp, "STOP") == 0)
                {
                    run_flag = 0;
                    if(log_flag == 1)
                        write(file, command, command_length);
                }
                else if(command_length == 4)
                {
                    if (strcmp(temp, "OFF") == 0)
                    {
                        if(log_flag == 1)
                            write(file, command, command_length);
                        button_pressed();
                    }
                    //else if (strcmp(temp, "LOG") == 0)
                }
                else
                    fprintf(stderr, "wrong command\n");

                strcpy(temp, command + command_length);
                strcpy(command, temp);
                pointer = strpbrk(command, "\n");
            }

            if(run_flag == 1)
            {
                value = mraa_aio_read(temperature_sensor);
                float R  = 1023.0/value-1.0;
                R = R0*R;
                celsius = 1.0 / (log(R/R0) / B + 1/298.15) - 273.15;
                char *output_string = malloc(sizeof(char) * 20);
                char *time_string = malloc(sizeof(char) * 10);
                memset(output_string, 0, 20);
                memset(time_string, 0, 10);
                get_current_time(&time_string);
                if (scale == 'F')
                {
                    fahrenheit = (celsius * 9) / 5 + 32;
                    sprintf(output_string, "%s %.1f\n", time_string, fahrenheit);
                }
                else
                    sprintf(output_string, "%s %.1f\n", time_string, celsius);
                fprintf(stdout, "%s", output_string);

                if (log_flag == 1)  //LOG
                {
                    write(file, output_string, 20);
                }
            }
            
        }
        else
        {
            read(fd, buf, sizeof(buf));
        }
    }

    /*  FIRST TRY
    int time_lapsed = 0;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while (run_flag)
    {
        value = mraa_aio_read(temperature_sensor);
        float R  = 1023.0/value-1.0;
        R = R0*R;
        celsius = 1.0 / (log(R/R0) / B + 1/298.15) - 273.15;

        if (time_lapsed >= period)
        {
            print_current_time();
            printf(" ");
            if (scale == 'F')
            {
                fahrenheit = (celsius * 9) / 5 + 32;
                fprintf(stdout, "%.1f\n", fahrenheit);
            }
            else
                fprintf(stdout, "temp: %.1f\n", celsius);
            
            clock_gettime(CLOCK_MONOTONIC, &start_time);
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        time_lapsed = end_time.tv_sec - start_time.tv_sec;
    }
    */

    
    return 0;
}