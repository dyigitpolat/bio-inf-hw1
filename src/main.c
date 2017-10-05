#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

char *input;
char *output;
int len; //length of a sequence
int N; //number of sequences
int k; //length of the kmer
uint32_t *sequences;

void parse_input();

int main( int argc, char** argv)
{
    if(argc != 3)
    {
        printf("invalid usage. run with arguments <input_file> <output_file> \n");
        return 1;
    }
    
    input = argv[1];
    output = argv[2];
    parse_input();
    
    return 0;
}

void parse_input()
{
    FILE* in;
    char c;

    N = 0;
    len = 0;
    in = fopen(input, "r"); if(!in) return;
    fscanf(in, "%d\n", &k);
    printf("k: %d\n", k);
    while((c = getc(in)) != '\n')
    {
        len++;
        printf("%c", c);
    }
    printf("- len: %d\n", len);
    N++;

    while((c = getc(in)) != EOF)
    {
        while((c = getc(in)) != '\n')
        {
            printf("%c", c);
        }
        printf("- len: %d\n", len);
        N++;
    }
    printf("N: %d\n", N);


    fclose(in);
    in = fopen(input, "r");

    

}