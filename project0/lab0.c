//NAME: Son Dang
//ID: 105215636
//EMAIL: sonhdang@ucla.edu
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <fcntl.h>	// sys calls read() and creat()
#include <unistd.h> //for write()
#include <signal.h>
#include <errno.h>
#include <string.h>

void segmentation_fault ()
{
	char* ch = NULL;
	*ch = '0';
	return;
}

void my_signal_handler(int sig)
{
	fprintf(stderr, "Segmentation fault on purpose with signal code: %d\n", sig);
	exit(4);
}

int main (int argc, char *argv[])
{
	int c;
	int isSegfault = 0;
	int ifd = 0;
	int ofd = 1;

	static struct option long_options[] = 
	{
		{"input", required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"segfault", no_argument, 0, 's'},
		{"catch", no_argument, 0, 'c'},
		{0, 0, 0, 0}
	};

	int option_index = 0;
	while (1)
	{
		c = getopt_long(argc, argv, "i:o:sc", long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
			case 'i':
			ifd = open(optarg, O_RDONLY);
			if(ifd >= 0) {
				close(0);
				dup(ifd);
				close(ifd);
				break;
			}
			else
			{
				fprintf(stderr, "--input: unable to open %s - %s\n", optarg, strerror(errno));
				exit(2);
			}
			case 'o':
			ofd = creat(optarg, 0666);
			if (ofd >= 0) {
				close(1);
				dup(ofd);
				close(ofd);
				break;
			}
			else
			{
				fprintf(stderr, "--output: unable to create %s\n", optarg);
				exit(3);
			}
			case 's':
			isSegfault = 1;
			break;
			case 'c':
			signal(SIGSEGV, my_signal_handler);
			break;
			case '?':
			exit(1);
			break;
			default:
			abort ();

		}
	}

	if (isSegfault == 1)
	{
		segmentation_fault();
	}


	char* ch = (char*) malloc(sizeof(char));
	int size = 0;
	while(*ch != EOF && *ch != '\n')
	{
		size = read(0, ch, sizeof(char));
		if (size == 0)
			break;
		write (1, ch, sizeof(char));
	}
	fclose(stdout);
	exit(0);

}