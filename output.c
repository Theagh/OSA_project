#include "output.h"

void initOutput(Output* out){
    out=malloc(sizeof *out);
    out->size=calloc(50*sizeof(char) );
    out->size=50;
}

void resize(Output* out,unsigned int newSize ){
    char* tmp=calloc(newSize*sizeof(char));
    for(int i=0;i<((out->size>newSize)?newSize:out->size);i++)
        tmp[i]=out->txt[i];
    free(out->txt);
    out->txt=tmp;
    out->size=newSize;
}