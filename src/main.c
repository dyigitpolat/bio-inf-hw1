#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

#define seq_get(x, y) (sequences[x*cols + (y >> 4)] >> ((y & 0x0F) << 1)) & 0x03
#define pat_get(x) (pattern >> (x << 1)) & 0x03
#define res_get(x) (result >> (x << 1)) & 0x03

char *input;
char *output;
int len; //length of a sequence
int N; //number of sequences
int k; //length of the kmer

int min_dist = 0;
int cols;
uint32_t sequences[8192*8192];
uint32_t pattern;

void parse_input();
uint32_t brute_force();

char num2letter(int x) 
{
    char c;
    switch( x) 
    { 
        case 0: c = 'A'; break; 
        case 1: c = 'T'; break; 
        case 2: c = 'G'; break; 
        case 3: c = 'C'; break; 
        default: c = 'X'; break; 
    }
    return c;
}

int main( int argc, char** argv)
{
    uint32_t result;
    if(argc != 3)
    {
        printf("invalid usage. run with arguments <input_file> <output_file> \n");
        return 1;
    }
    
    input = argv[1];
    output = argv[2];
    parse_input();
    result = brute_force();

    FILE* out;
    out = fopen(output, "w+"); if(!out) return 1;

    printf("dist:%d median: \n", min_dist);
    int x;
    for( x = 0; x < k; x++)
    {
        printf("%c", num2letter( res_get(x)));
        putc(num2letter( res_get(x)), out);
    }
    fclose(out);
    printf("\n");

    return 0;
}

int hamming(int i, int j)
{
    int hdist = 0;
    int x;
    hdist = 0;
    for( x = 0; x < k; x++)
    {
        //if(pattern == 0x0540) printf("%d %d \n", pat_get(x), seq_get(i, (j+x)) );
        if( (pat_get(x)) ^ (seq_get(i, (j+x))) )
        {
            hdist++;
        }
    }
    return hdist;
}

uint32_t brute_force()
{
    int i, j;
    int dist;
    int tmp;
    int mdist;

    min_dist = INT_MAX;
    mdist = INT_MAX;

    uint32_t best_pat = 0;
    for(pattern = 0; pattern < (1 << (k*2)); pattern++)
    {
        dist = 0;
        for(i = 0; i < N; i++)
        {
            mdist = INT_MAX;
            for(j = 0; j <= len - k; j++)
            {
                tmp = hamming(i, j);
                if(tmp < mdist)
                {
                    mdist = tmp;
                }
            }
            dist += mdist;

            if( dist >= min_dist)
            {
                break;
            }
        }

        if( dist < min_dist)
        {
            min_dist = dist;
            best_pat = pattern;
        }
    }
    return best_pat;
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
    }
    printf("len: %d\n", len);
    N++;

    while(c != EOF)
    {   
        while((c = getc(in)) != '\n' && c != EOF);
        N++;
    }

    printf("N: %d\n", N);

    fclose(in);
    in = fopen(input, "r");
    fscanf(in, "%d\n", &k);
    cols = (len / 16) + 1;
    
    int i, j;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < len + 1; j++) // +1 because \n do nothing.
        {
            int a;
            switch(getc(in))
            {
                case 'A': a = 0; break;
                case 'T': a = 1; break;
                case 'G': a = 2; break;
                case 'C': a = 3; break;
                case '\n': a = -1; break;
            }
            if(a != -1)
                sequences[i*cols + (j / 16)] += a << ((j % 16) * 2);
        }
    }

    //test access
    /*
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < len; j++)
        {
            char c;
            switch( seq_get(i, j)) 
            {
                case 0: c = 'A'; break;
                case 1: c = 'T'; break;
                case 2: c = 'G'; break;
                case 3: c = 'C'; break;
                default: c = 'X'; break;
            }
            printf("%c",c);
        }
        printf("\n");
    }
    */
}