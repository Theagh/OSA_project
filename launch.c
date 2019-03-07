#include "launch.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>
#include<fcntl.h> 

void mssleep(unsigned long ms)
{
    usleep(ms*1000);
}

int launch(char** argv,Output* output)
{
    int ret_val;
    int tube[2];
    if(pipe(tube)==-1){
        perror("Couldn't create pipe");
        return -1;
    }
    switch (fork())
    {
        case -1:
            perror("Fork error");
            return -1;
        case 0:
            // printf("before child fork\n");
            dup2(tube[1],1);
            // fcntl(1, F_SETFL, O_NONBLOCK);
            // close(3);
            // printf("before child fork\n");

            if(execvp(argv[0], &argv[0] )==-1){
                // fprintf(stderr,"%s %s\n", argv[0], argv[1]);
                perror("execv");
            }
        default:;
            int rd=0, rd_tmp=0;
            close(tube[1]);
            // fcntl(tube[0], F_SETFL, O_NONBLOCK);
            while( ( rd_tmp+=read(tube[0], &output->txt[rd],128) )> 0)
            {
                rd+=rd_tmp;
                rd_tmp=0;
                if( rd >= output->size )
                    resize( &output, rd+128 );

            }
            wait(&ret_val);
            if(WIFEXITED(ret_val))
                return WEXITSTATUS(ret_val);
            else return -1;
    }
}