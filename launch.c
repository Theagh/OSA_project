#include "launch.h"
#include <sys/wait.h>
#include <unistd.h>

int launch(const char* argv[],char* output){
    
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
            close(1);
            dup(tube[1]);
            execv(argv, &argv[1] );
        default:;
            int rd=0;
            while( rd+=read(0, &output[rd],128)>0);  
            wait(&ret_val);
            if(WIFEXITED(ret_val))
                return WEXITSTATUS(ret_val);
            else return -1;
    }
}