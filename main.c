#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <sys/select.h>
#include <string.h>
#include "launch.h"
#include "output.h"

#define DEF_INTERVAL 10000

void print_time(char *time_format){
    char timestr[64];
    time_t rawt = time(NULL);
    struct tm *tp = localtime(&rawt);
    strftime(timestr, 64, time_format, tp);
    printf("%s\n", timestr);
}

int main(int argc, char **argv)
{
    char c;
    int interval=DEF_INTERVAL, limit=0;
    bool ret_code=false;
    bool time_flag=false;
    if(argc==1 ) { // if no arguments are given, exit 
        fprintf(stderr, "No arguments\n");
        exit(EXIT_FAILURE);
    }
    char* time_format; //to store the time format from argv
	while((c=getopt(argc, argv, "+t:i:l:c"))!=-1){ //Acquire options
		
		switch (c)
		{
			case 'i':
                interval=atoi(optarg);
                if( interval <= 0 ){
                    fprintf(stderr, "Interval should be more than 0.\n");
                    exit(EXIT_FAILURE);
                }
                break;
			case 'l':
                limit=atoi(optarg);
                if( limit < 0 ){
                    fprintf(stderr, "Limit should be non-negative.\n");
                    exit(EXIT_FAILURE);
                }
				break;
            case 'c':
                ret_code=true;
				break;
            case 't':
                time_format=optarg;
                time_flag=true;
				break;

			default:
                printf("exiting");
				exit(1);
				break;
		}
	}
    int read_n; // To store the number of bytes read to output buffer
    char ** cmd=argv+optind; // stores the pointer to the beggining of argv array, from where we will execute the command
    if( argc-optind==0 ){
        fprintf(stderr,"No executable specified\n");
        exit(EXIT_FAILURE);
    }
    Output *out1=initOutput(),*out2=initOutput(); // Initialize the output buffers
    /*
        This is the beginning of the first iteration
    */
    if( time_flag ) print_time(time_format); //if -t is specified print time with time_format
    int r_code, r_codeOld=launch(cmd, out1, &read_n);
    copyOutput(out1,out2);
    write(STDOUT_FILENO,out1->txt,read_n);

    if(ret_code) printf("exit %d\n", r_codeOld);
    fflush(stdout);
    if(limit==1) return 0;
    mssleep(interval);
    for(int i=1;(limit==0)?(i>limit):(i<limit);i++){
        if( time_flag ) print_time(time_format);

        int r_code=launch(cmd, out1, &read_n);
        if( compareOutput(out1,out2) ) {
            write(STDOUT_FILENO,out1->txt,read_n);
            copyOutput(out1,out2); // Copies out1->txt to out2->txt
        }
        if(ret_code && ( r_code!=r_codeOld )) { // if -c is specified and current return code is different than the previous return code
            printf("exit %d\n", r_code);
            fflush(stdout);
            r_codeOld=r_code;
        }
        freeOutput(&out1); //free and reinitialize the out1 buffer
        out1=initOutput();
        mssleep(interval);
    }
    freeOutput(&out2);
    freeOutput(&out1);
    return 0;
}
