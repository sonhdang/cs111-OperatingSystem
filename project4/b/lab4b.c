#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef DUMMY
#define MRAA_GPIO_IN 0
#define MRAA_SUCCESS 1
#define MRAA_GPIO_EDGE_RISING 1
typedef int mraa_aio_context;
typedef int mraa_gpio_context;
typedef int mraa_result_t;
typedef int mraa_gpio_edge_t;
typedef int mraa_gpio_dir_t;
int mraa_aio_read(mraa_aio_context c)
{
    return 650;
}
mraa_result_t mraa_aio_close(mraa_aio_context c)
{
    return 1;
}
mraa_result_t mraa_gpio_close(mraa_gpio_context dev)
{
    return 1;
}
mraa_gpio_context mraa_gpio_init(int pin)
{
    return 1;
}
mraa_aio_context mraa_aio_init(unsigned int pin)
{
    return 1;
}
mraa_result_t 	mraa_gpio_isr (mraa_gpio_context dev, mraa_gpio_edge_t edge, void(*fptr)(void *), void *args)
{
    return 1;
}
void mraa_deinit()
{
    return;
}
mraa_result_t mraa_gpio_dir (mraa_gpio_context dev, mraa_gpio_dir_t dir)
{
    return 1;
}
#else
#include <mraa.h>
#endif

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
// Temperature sensor
int value = 0;
float float_value = 0;
const int R0 = 100000;
const int B = 4275;
float celsius = 0;
float fahrenheit = 0;
// Others
char scale = 'F';   // Default scale set to Fahrenheit (F)
int period = 1;     // Default sampling interval 1 read/sec
char* filename;
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
    exit(EXIT_SUCCESS);
}

void get_temp()
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

int main(int argc, char* argv[])
{

    /* READING OPTION FROM THE TERMINAL */
    int c;
    while(1)
    {
        static struct option long_options[] =
        {
            {"period", required_argument, NULL, 'p'},
            {"scale", required_argument, NULL, 's'},
            {"log", required_argument, NULL, 'l'},
            {0, 0, 0, 0}
        };

        c = getopt_long_only(argc, argv, "", long_options, NULL);

        if (c == -1)
            break;
        
        switch (c)
        {
            case 'p':       // Sampling Rate
                // if (sscanf(optarg, "%d", &period) == 0)
                // {
                //     fprintf(stderr, "Argument needs to be an integer\n");
                //     exit(EXIT_FAILURE);
                // }
                sscanf(optarg, "%d", &period);
                break;
            case 'l':       // Log file
                filename = malloc(sizeof(char) * 20);
                strcpy(filename, optarg);
                log_flag = 1;
                break;
            case 's':       // Scale
                if (strlen(optarg) == 1 && (optarg[0] == 'C' || optarg[0] == 'F'))
                {
                    scale = optarg[0];
                    break;
                }
                else
                {
                    fprintf(stderr, "Argument should either be 'C' or 'F'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
				fprintf(stderr, "Unknown Argument\n");
				exit(EXIT_FAILURE);
				break;
        }
    }

    /* LOG FILE */
    if (log_flag == 1)
    {
        if (filename == NULL)
        {
            fprintf(stderr, "Error with filename\n");
            exit(1);
        }
        else
        {
            file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
    }
    
    
    /* BUTTON */
    button = mraa_gpio_init(73);
    mraa_gpio_dir(button, MRAA_GPIO_IN);
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &button_pressed, NULL);


    /* TEMPERATURE SENSOR */
    temperature_sensor = mraa_aio_init(0);
    if (temperature_sensor == NULL) {
        fprintf(stderr, "Failed to initialize AIO\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* MAIN PROGRAM */
    int fd = 0;
    int pret;
    int timeout;
    struct pollfd *pollfds = malloc(sizeof(*poll));
    pollfds->fd = fd;
    pollfds->events = 0;
    pollfds->events |= POLLIN;
    
    

    char buf[100];
    char command[100];
    memset(command, 0, 100);    //initializing the string 'command'
    memset(buf, 0, 100);
    char* pointer;              //points to the position of '\n' in the string
    
    
    
    while(1)
    {
        strcat(command, buf);   //concatenating input after each read
        memset(buf, 0, 100);    //clearing buffer after each read;
        pointer = strpbrk(command, "\n");
        timeout = period * 1000;
        pret = poll(pollfds, 1, timeout);
        if(pret == 0)
        {
            if(run_flag == 1)
            {
                get_temp();
            }

        }
        else
        {
            read(fd, buf, sizeof(buf));

            while (pointer != NULL)
            {
                int command_length = pointer - command + 1;
                char temp[100];
                memset(temp, 0, 100); //initializing string 'temp'
                strncpy(temp, command, command_length - 1); //getting the command
                if(log_flag == 1)
                            write(file, command, command_length);
                if (command_length < 3)
                {
                    fprintf(stderr, "Command invalid\n");
                    exit(EXIT_FAILURE);
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
                    }
                    memcpy(check, &temp[0], 6);
                    check[6] = '\0';
                    if(strcmp(check, "SCALE=") == 0)
                    {
                        if (temp[6] == 'C' || temp[6] == 'F')
                        {
                            scale = temp[6];
                        }
                        else
                        {
                            fprintf(stderr, "%c is not a valid scale\n", temp[7]);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else if(command_length == 6 || strcmp(temp, "START") == 0)
                {
                    run_flag = 1;
                }
                else if(command_length == 5 || strcmp(temp, "STOP") == 0)
                {
                    run_flag = 0;
                }
                else if(command_length == 4)
                {
                    if (strcmp(temp, "OFF") == 0)
                    {
                        button_pressed();
                        exit(EXIT_SUCCESS);
                    }
                    else if (strcmp(temp, "LOG") == 0)
                    {
                        log_flag = 0;
                    }
                    else
                    {
                        fprintf(stderr, "wrong command\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    fprintf(stderr, "wrong command\n");
                    exit(EXIT_FAILURE);
                }

                strcpy(temp, command + command_length);
                strcpy(command, temp);
                pointer = strpbrk(command, "\n");
            }
        }
    }
    
    return 0;
}