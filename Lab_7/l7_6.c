#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include <time.h>
#include<sys/stat.h>

char *permisiuniFisier(mode_t p)
{
    char *aux = (char*) malloc(10);
    if(p & S_IRUSR) aux[0] = 'R'; else aux[0] = '-';
    if(p & S_IWUSR) aux[1] = 'W'; else aux[1] = '-';
    if(p & S_IXUSR) aux[2] = 'X'; else aux[2] = '-';
    if(p & S_IRGRP) aux[3] = 'R'; else aux[3] = '-';
    if(p & S_IWGRP) aux[4] = 'W'; else aux[4] = '-';
    if(p & S_IXGRP) aux[5] = 'X'; else aux[5] = '-';
    if(p & S_IROTH) aux[6] = 'R'; else aux[6] = '-';
    if(p & S_IWOTH) aux[7] = 'W'; else aux[7] = '-';
    if(p & S_IXOTH) aux[8] = 'X'; else aux[8] = '-';
    
    aux[9] = '\0';
    return aux;
}

void afisareFisier(int f_out, char *vnume, char *nume)
{
    
    int f_open;
    char path[100];
    
    snprintf(path, sizeof(path), "%s/%s", vnume, nume);
    
    struct stat s;
    lstat(path, &s);
    
    if ((f_open = open(path, O_RDONLY)) < 0)
    { 
        perror("r1"); 
        exit(1); 
    } 
    
    int latime, inaltime;

    lseek(f_open, 18, SEEK_SET);
    
    read(f_open, &latime, sizeof(int));
    read(f_open, &inaltime, sizeof(int));
    
    char buff[100];
    
    sprintf(buff, "nume fisier: %s\n",nume);
    write(f_out, buff, strlen(buff));
    
    if(strstr(nume, ".bmp") != NULL)
    {
        sprintf(buff, "inaltime: %d\n",latime);
        write(f_out, buff, strlen(buff));
    
        sprintf(buff, "lungime: %d\n",inaltime);
        write(f_out, buff, strlen(buff));
    }
    
    sprintf(buff, "dimensiune: %ld\n", s.st_size);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "identificatorul utilizatorului: %d\n", s.st_uid);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "timpul ultimei modificari: %s", ctime(&s.st_mtime));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "contorul de legaturi: %ld\n", s.st_nlink);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces user: %.3s\n", permisiuniFisier(s.st_mode & S_IRWXU));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces grup: %.3s\n", permisiuniFisier((s.st_mode & S_IRWXG)<<3));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces altii: %.3s\n\n", permisiuniFisier((s.st_mode & S_IRWXO)<<6));
    write(f_out, buff, strlen(buff));
    
    close(f_open);
}

void scriereDirector(int f_out, char *vnume, char *nume)
{
    char buff[100];
    char path[100];
    
    snprintf(path, sizeof(path), "%s/%s", vnume, nume);
    struct stat s;
    lstat(path, &s);
    
    sprintf(buff, "nume director: %s\n",nume);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "identificatorul utilizatorului: %d\n", s.st_uid);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces user: %.3s\n", permisiuniFisier(s.st_mode & S_IRWXU));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces grup: %.3s\n", permisiuniFisier((s.st_mode & S_IRWXG)<<3));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces altii: %.3s\n\n", permisiuniFisier((s.st_mode & S_IRWXO)<<6));
    write(f_out, buff, strlen(buff));
}

void scriereLegatura(int f_out, char *vnume, char *nume)
{
    char buff[100];
    char path[100];
    
    snprintf(path, sizeof(path), "%s/%s", vnume, nume);
    struct stat s;
    lstat(path, &s);
    
    sprintf(buff, "nume legatura: %s\n",nume);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "dimensiune legatura: %ld\n", s.st_size);
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces user: %.3s\n", permisiuniFisier(s.st_mode & S_IRWXU));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces grup: %.3s\n", permisiuniFisier((s.st_mode & S_IRWXG)<<3));
    write(f_out, buff, strlen(buff));
    
    sprintf(buff, "drepturi de acces altii: %.3s\n\n", permisiuniFisier((s.st_mode & S_IRWXO)<<6));
    write(f_out, buff, strlen(buff));
}

int main(int argc, char *argv[])
{
    if(argc != 2 && strstr(argv[1], ".bmp") != NULL)
    {
        printf("Erroare!");
        exit(1);
    }
    
    int f_out;
    if((f_out = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) < 0)
    {
        perror("r2"); 
        exit(1); 
    }
    /*struct stat s;
    lstat(argv[1], &s);
    afisareFisier(s, f_out, argv[1]);*/
    
    struct stat s;
    DIR *dir = opendir(argv[1]);
    if(dir == NULL)
    {
        perror("r3");
        exit(1);
    }
    
    struct dirent *i;
    
    while((i = readdir(dir)) != NULL)
    {
        lstat(i->d_name, &s);
        
        if(strcmp(i->d_name, ".") != 0 &&  strcmp(i->d_name, "..") != 0)
        {
            if(i->d_type == DT_DIR)
                scriereDirector(f_out, argv[1], i->d_name);
            else if(i->d_type == DT_LNK)
                scriereLegatura(f_out, argv[1], i->d_name);
            else if(i->d_type == DT_REG)
                afisareFisier(f_out, argv[1], i->d_name);
        }
    }
    
    
    close(f_out);
    
    return 0;
}
