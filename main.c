#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <sys/select.h>

#include "launch.h"
#include "output.h"

#define DEF_INTERVAL 10000

int main(int argc, char *const *argv)
{
    char c;
    unsigned int interval=DEF_INTERVAL, limit=0;
    bool ret_code=false;
    char* time_format=malloc(50);
	while((c=getopt(argc, argv, "+t:i:l:c"))!=-1){
		
		switch (c)
		{
			case 'i':
                interval=atoi(optarg);
                break;
			case 'l':
                limit=atoi(optarg);
				break;
            case 'c':
                ret_code=true;
				break;
            case 't':
                time_format=optarg;
				break;
			case '?':
				// help();
				break;
            case ':':
            if ( optopt=='t')
                fprintf(stderr,"Please, specify a time format after -%c\n", optopt);
            else if ( optopt=='i') 
                fprintf(stderr,"Please, specify a waiting interval after -%c\n", optopt);
            else if ( optopt=='l') 
                fprintf(stderr,"Please, specify max number of launches after -%c\n", optopt);
            exit(-1);
			default:
				exit(-1);
				break;
		}
	}
    char **cmd=argv+optind;
    int i=0;
    while(cmd[i]){
        printf("%s ", cmd[i++]);
    }
    
    Output *out1=initOutput(),*out2=initOutput();
    printf("\nrun #%d\n", i);
    int r_code=launch(cmd, out1);
    copyOutput(out1,out2);
    write(1,out1->txt, out1->size);

    for(int i=1;i<limit;i++){
        printf("\nrun #%d\n", i);
        int r_code=launch(cmd, out1);
        copyOutput(out1,out2);
        write(1,out1->txt, out1->size);
        freeOutput(&out1);
        out1=initOutput();
        mssleep(interval);
    }
    
    freeOutput(&out2);
    freeOutput(&out1);
    free(time_format);
    return 0;
}
