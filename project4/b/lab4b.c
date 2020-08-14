#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <mraa.h>

void button_pressed()
{
    printf("BUTTON PRESSED\n");
}

int main(int argc, char* argv[])
{
    /* DECLARING VARIABLES */
    char scale = 'F';   // Default scale set to Fahrenheit (F)
    int period = 1;     // Default sampling interval 1 read/sec

    /* READING OPTION FROM THE TERMINAL */
    int c;

    while(1)
    {
        static struct option long_options[] =
        {
            {"period", optional_argument, NULL, 'p'},
            {"scale", optional_argument, NULL, 's'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        if (c == -1)
            break;
        
        switch (c)
        {
            case 'p':
                if (sscanf(optarg, "%d", &period) == 0)
                {
                    printf("input needs to be an integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
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
        }
    }

    /* MAIN PROGRAM */
    printf("MRAA version: %s\n", mraa_get_version());

    /* BUTTON */
    mraa_result_t status;
    mraa_gpio_context button;
    button = mraa_gpio_init(73);
    mraa_gpio_dir(button, MRAA_GPIO_IN);
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &button_pressed, NULL);


    /* TEMPERATURE SENSOR */
    int value = 0;
    float float_value = 0;
    const int R0 = 100000;
    const int B = 4275;
    
    mraa_aio_context temperature_sensor;
    temperature_sensor = mraa_aio_init(0);
    if (temperature_sensor == NULL) {
        fprintf(stderr, "Failed to initialize AIO\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }
    while (1) {
        sleep(2);
        value = mraa_aio_read(temperature_sensor);
        float R  = 1023.0/value-1.0;
        R = R0*R;
        float temperature = 1.0/(log(R/R0) / B + 1/298.15)-273.15;
        fprintf(stdout, "temp: %f\n", temperature);
    }
    /* close AIO */
    status = mraa_aio_close(temperature_sensor);
    if (status != MRAA_SUCCESS) {
        exit(EXIT_FAILURE);
    }
    mraa_gpio_close(button);
    
    return 0;
}