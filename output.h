#ifndef OUTPUT_
#define OUTPUT_
typedef struct
{
    char* txt;
    unsigned int size;
}Output;

void initOutput(Output* out);
void resize(Output* out, int newSize );
#endif