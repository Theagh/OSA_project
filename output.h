#ifndef OUTPUT_
#define OUTPUT_
typedef struct
{
    char* txt;
    unsigned int size;
}Output;

Output* initOutput();
void resize(Output** out,unsigned int newSize );
void copyOutput(Output* out1,Output* out2);
int compareOutput( Output* out1 ,Output* out2 );
void freeOutput(Output** out);

#endif