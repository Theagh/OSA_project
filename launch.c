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

int launch(char** argv,Output* output, int* read_bytes)
{
    int ret_val, tube[2];
    if(pipe(tube)==-1){
        perror("Couldn't create pipe");
        return -1;
    }
    switch (fork())
    {
        case -1:
            perror("Fork error");
            return -1;
        case 0://Child
            dup2(tube[1],1);
            close(3);
            close(tube[0]);
            close(tube[1]);
            execvp(argv[0], &argv[0]);
            break;
        default:;//Parent
            int rd=0, rd_tmp=0;
            close(tube[1]);
            while( ( rd_tmp+=read(tube[0], &output->txt[rd],128-rd%128) )> 0)
            {
                rd+=rd_tmp;
                rd_tmp=0;
                if( rd > (int) output->size-1 )
                    resize( &output, rd*2 );
            }
            *read_bytes=rd;
            wait(&ret_val);
            if(WIFEXITED(ret_val))
                return WEXITSTATUS(ret_val);
            else return -1;
    }
}