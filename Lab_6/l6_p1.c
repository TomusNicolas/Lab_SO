#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <ctype.h>
#include <string.h>



int main(int argc, char *argv[])
{
    int f_open, f_out;
    int sz;
    char c[100], scris[200];
    int nr_mici = 0, nr_mari = 0, cifre = 0, ch_gasit = 0, dimensiune = 0;

    if(argc != 4)
    {
        printf("Icorect\n");
        return -1;
    }

    f_open = open(argv[1], O_RDONLY);
    if (f_open < 0) { 
        perror("r1"); 
        exit(1); 
    } 

    f_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (f_out < 0) { 
        perror("r2"); 
        exit(1); 
    } 

    sz = read(f_open, c, 10);
    while(sz)
    {
        c[sz] = '\0';
        //printf("%s\n\n", c);

        for(int i = 0; i < sz; i++)
        {
            if(islower(c[i]))
                nr_mici++;
            else if(isupper(c[i]))
                nr_mari++;
            else if(isdigit(c[i]))
                cifre++;
            
            if(argv[3][0] == c[i])
                ch_gasit++;
        }
        dimensiune += sz;
        sz = read(f_open, c, 10);
    }

    sprintf(scris, "numar litere mici: %d\nnumar litere mari: %d\nnumar cifre: %d\nnumar aparitii caracter: %d\ndimensiune fisier: %d\n", nr_mici, nr_mari, cifre, ch_gasit, dimensiune);
    
    write(f_out, scris, strlen(scris));

    if(close(f_open) == -1)
    {
        printf("Fisierul nu a putut fi inchis");
        exit(1); 
    } 

    if(close(f_out) == -1)
    {
        printf("Fisierul nu a putut fi inchis");
        exit(1); 
    } 
    return 0;
}
