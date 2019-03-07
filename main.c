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

int main(int argc, char *const *argv)
{
    char c;
    int interval=DEF_INTERVAL, limit=0;
    bool ret_code=false;
    if(argc==1 ) {
        fprintf(stderr, "No arguments\n");
        exit(EXIT_FAILURE);
    }
    char* time_format;
	while((c=getopt(argc, argv, "+t:i:l:c"))!=-1){
		
		switch (c)
		{
			case 'i':
                interval=atoi(optarg);
                if( interval <= 0 ){
                    fprintf(stderr, "Interval should be more than 0. %d\n");
                    exit(EXIT_FAILURE);
                }
                break;
			case 'l':
                limit=atoi(optarg);
                if( limit < 0 ){
                    fprintf(stderr, "Limit should be non-negative. \n");
                    exit(EXIT_FAILURE);
                }
				break;
            case 'c':
                ret_code=true;
				break;
            case 't':
                time_format=optarg;
                
				break;
			case '?':
                fprintf(stderr,"Please, specify a time format after\n");
				break;
            case ':':
            
            exit(1);
			default:
                printf("exiting");
				exit(1);
				break;
		}
	}

    char ** cmd=argv+optind;
    if( argc-optind==0 ){
        fprintf(stderr,"No executable specified\n");
        exit(EXIT_FAILURE);
    }
    
    Output *out1=initOutput(),*out2=initOutput();

    int r_code, r_codeOld=launch(cmd, out1);
    copyOutput(out1,out2);
    write(1,out1->txt, out1->size);
    if(ret_code) printf("exit: %d", r_codeOld);
    for(int i=1;i<limit;i++){
        printf("\nrun #%d\n", i);
        int r_code=launch(cmd, out1);
        if( compareOutput(out1,out2) || r_code!=r_codeOld ){
            write(1,out1->txt, out1->size);
            copyOutput(out1,out2);
        }
        r_codeOld=r_code;
        if(ret_code) printf("exit: %d", r_codeOld);

        freeOutput(&out1);
        out1=initOutput();
        mssleep(interval);
    }
    
    freeOutput(&out2);
    freeOutput(&out1);
    free(time_format);
    return 0;
}
