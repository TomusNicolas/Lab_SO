#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>

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
        exit(-1); 
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

void bmpToGrayScale(char *vnume, char *nume)
{
    int f_open;
    char path[100];
    int latime;
    int inaltime;
    unsigned char pixel[3];
    
    snprintf(path, sizeof(path), "%s/%s", vnume, nume);
    
    if ((f_open = open(path, O_RDWR)) < 0)
    { 
        perror("r1"); 
        exit(-1); 
    } 
    
    lseek(f_open, 18, SEEK_SET);
    
    read(f_open, &latime, sizeof(int));
    read(f_open, &inaltime, sizeof(int));
    
    lseek(f_open, 54, SEEK_SET);
    
    
    for(int i = 0; i < (latime * inaltime); i++)
    {
        read(f_open, pixel, sizeof(pixel));
        pixel[0] = (0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        pixel[1] = (0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        pixel[2] = (0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        lseek(f_open, -3, SEEK_CUR);
        write(f_open, pixel, sizeof(pixel)); 
    }
    
    close(f_open);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Erroare!");
        exit(-1);
    }
    
    int f_out;
    
    struct stat s;
    DIR *dir = opendir(argv[1]);
    if(dir == NULL)
    {
        perror("r3");
        exit(-1);
    }
    
    struct dirent *i;
    char out_path[300];
    int cod;
    
    while((i = readdir(dir)) != NULL)
    {
        int nr_linii = 0;
        lstat(i->d_name, &s);
        
        sprintf(out_path, "%s/%s_statistica.txt", argv[2], i->d_name);
        
        if((f_out = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) < 0)
        {
            perror("r2"); 
            exit(-1); 
        }
        
        pid_t pid = fork();
        if(pid == 0)
        {
            if(strcmp(i->d_name, ".") != 0 &&  strcmp(i->d_name, "..") != 0)
            {
                if(i->d_type == DT_DIR)
                {
                    nr_linii = 6;
                    scriereDirector(f_out, argv[1], i->d_name);
                }
                else if(i->d_type == DT_LNK)
                {
                    nr_linii = 5;
                    scriereLegatura(f_out, argv[1], i->d_name);
                }
                else if(i->d_type == DT_REG)
                {
                    if(strstr(i->d_name, ".bmp") != NULL)
                    {
                        nr_linii = 10;
                        afisareFisier(f_out, argv[1], i->d_name);
                        pid_t bmp = fork();
                        
                        if(bmp == 0)
                        {
                            bmpToGrayScale(argv[1], i->d_name);
                            exit(100);
                        }
                        else if(bmp < 0)
                        {
                            printf("Eroare BMP\n");
                            exit(-1);
                        }
                        
                        int status;
                        int pid;
                        
                        pid = wait(&status);
                        
                        if(pid == -1)
                        {
                            printf("Eroare FORK\n");
                            exit(-1);
                        }
                        
                        if (WIFEXITED(status)) 
                        {
                            cod = WEXITSTATUS(status);
                            printf("S-a încheiat procesul cu pid-ul %d și codul %d\n", pid, cod);
                        }
                        
                    }
                    else
                    {
                        afisareFisier(f_out, argv[1], i->d_name);
                        nr_linii = 8;
                    }
                }
            }
            exit(nr_linii);
        }
        
    }
    
    int status;
    int pid;
    while (waitpid(-1, &status, 0) > 0) 
    {
        pid = waitpid(-1, &status, 0);
        if (WIFEXITED(status))
        {
            cod = WEXITSTATUS(status);
            printf("S-a încheiat procesul cu pid-ul %d și codul %d\n", pid, cod);
        }
    }
    
    
    close(f_out);
    
    return 0;
}
