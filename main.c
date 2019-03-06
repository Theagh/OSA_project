#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>


#define DEF_INTERVAL 10000

int main(int argc, char const *argv[])
{
    char c;
    unsigned int interval=DEF_INTERVAL, limit=0;
    bool ret_code=false;
    const char* time_format=malloc(50);
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
            
			default:
				exit(-1);
				break;
		}
	}
    return 0;
}
