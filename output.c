#include "output.h"
#include <stdlib.h>
#include <string.h>

Output* initOutput(){
    Output* out=malloc(sizeof *out);
    out->txt=calloc(128,sizeof(char) );
    out->size=128;
    return out;
}

void resize(Output** out,unsigned int newSize ){
    char* tmp=calloc(newSize,sizeof(char));
    for(int i=0;i<(( (*out)->size>newSize)?newSize: (*out)->size);i++)
        tmp[i]=(*out)->txt[i];
    free((*out)->txt);
    (*out)->txt=tmp;
    (*out)->size=newSize;
}
//Copies out1 txt into out2
void copyOutput(Output* out1,Output* out2)
{
    if( out2->size< out1->size){
        realloc( out2->txt, out1->size );
        out2->size=out1->size;
    }
    for(int i = 0; i < out1->size ; i++)
        out2->txt[i]=out1->txt[i];
}

int compareOutput( Output* out1 ,Output* out2 ){
    if( strlen(out1->txt)!=strlen(out2->txt) )
        return 0;
    
    return strncmp(out1->txt,out2->txt, out1->size );
    
}

void freeOutput(Output** out)
{
    free((*out)->txt );
    free( *out );
}