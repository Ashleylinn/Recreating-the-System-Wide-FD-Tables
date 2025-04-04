#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "functions.h"

void composite_table(int PID, int TXT, int binary){
    ///_|> descry: this function will only display the composed table 
    ///_|> PID: given PID if speicfied, if no specified then it is -1
    ///_|> TXT: if there is a --output_TXT argument then will TXT = 1 
    ///_|> binary: if --output_binary argument then will binary = 1
    ///_|> returning: this function will not return anything

    DIR *dir; //create a pointer pointing to a directory
    struct dirent *read_dir; //create a pointer to a structure of dirent 
    int count = 0; //create variable for the index 

    FILE *txtFile = NULL; 
    if(TXT == 1){ //if flag --output_TXT then write to the compositeTable.txt
        txtFile = fopen("compositeTable.txt", "w");
        if(txtFile == NULL){
            perror("Cannot open the file");
            return;
        }
        fprintf(txtFile, "         %-6s   %-8s %-50s %-10s\n", "PID", "FD", "Filename", "Inode");
        fprintf(txtFile, "         ===============================================================================\n"); 
    }

    FILE *bFile = NULL;
    if(binary == 1){ //if --output_binary then write to compositeTable.bin
        bFile = fopen("compositeTable.bin", "wb");
        if(bFile == NULL){
            perror("Cannot open the file");
            return;
        }
        char title[] = "           PID       FD         Filename                                              Inode\n" 
                       "           ================================================================================\n";
        fwrite(title, sizeof(title), 1, bFile); 
    }

    printf("         %-6s   %-8s %-50s %-10s\n", "PID", "FD", "Filename", "Inode");
    printf("         =================================================================================\n");
    

    //if there is a specified PID 
    if(PID > 0){ 
        char path[4096]; //create a string that store the path to the directory of /proc/%d/fd  
        snprintf(path, sizeof(path), "/proc/%d/fd", PID);
        dir = opendir(path);
        if(dir == NULL){
            perror("Cannot open the directory"); 
            return;
        }

        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if the name of entry is not a numeric character
            }
            
            char fdlink[4096]; //create a string to get filenmae
            snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", PID, read_dir->d_name);

            char fname[4096];
            ssize_t len = readlink(fdlink, fname, sizeof(fname) - 1); //readlink place the info of symbolic link to the fname 
            if(len == -1){
                perror("readlink");
                continue;
            }   
            fname[len] = '\0'; //add null terminator to fname since it does not include when using readlink
            
            int fd = atoi(read_dir->d_name); //convert to integer
            //get the number of inode
            struct stat sfile;
            if(stat(fdlink, &sfile) == 0) { //use stat to get information about the file pointed to by pathname
                printf("%-8d %-8d %-8d %-50s %-10lu\n", count++, PID, fd, fname, sfile.st_ino);

                if(txtFile){//write into the compositeTable.txt
                    fprintf(txtFile, "%-8d %-8d %-8d %-50s %-10lu\n", count, PID, fd, fname, sfile.st_ino);
                }
                if(bFile){ //write into the compositeTable.bin
                    fwrite(&count, sizeof(int), 1, bFile);
                    fwrite(&PID, sizeof(int), 1, bFile);
                    fwrite(&fd, sizeof(int), 1, bFile);
                    fwrite(fname, sizeof(char), strlen(fname) + 1, bFile);
                    fwrite(&sfile.st_ino, sizeof(unsigned long), 1, bFile);
                }
            }
        }
        closedir(dir);
        
    }else{ //if PID = -1 (there is no specified PID)
        dir = opendir("/proc"); //getting all the PIDs
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }

        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if is not a numeric character
            }                
            int pid = atoi(read_dir->d_name); //convert to integer 
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);

            DIR *fd_dir = opendir(path); 
            if(fd_dir == NULL){
                continue; 
            }
            
            struct dirent *read_fd;
            while((read_fd = readdir(fd_dir)) != NULL){
                if(read_fd->d_name[0] < '0' || read_fd->d_name[0] > '9'){
                        continue;
                }

                char fdlink[4096]; //create a string to get filenmae
                snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", pid, read_fd->d_name);
                
                char fname[4096]; 
                ssize_t len = readlink(fdlink, fname, sizeof(fname) - 1); //readlink place the info of symbolic link to the fname 
                if(len == -1){
                    continue;
                }   
                fname[len] = '\0';  //add null terminator to fname since it does not include when using readlink

                int fd = atoi(read_fd->d_name); //convert the FD names to integers
                struct stat sfile;
                if(stat(fdlink, &sfile) == 0){
                    printf("%-8d %-8d %-8d %-50s %-10lu\n", count++, pid, fd, fname, sfile.st_ino);

                    if(txtFile) { //write into the compositeTablt.txt
                        fprintf(txtFile, "%-8d %-8d %-8d %-50s %-10lu\n", count, pid, fd, fname, sfile.st_ino);
                    }

                    if(bFile){ //write into the compositeTablt.bin
                        fwrite(&count, sizeof(int), 1, bFile);
                        fwrite(&pid, sizeof(int), 1, bFile);
                        fwrite(&fd, sizeof(int), 1, bFile);
                        fwrite(fname, sizeof(char), strlen(fname) + 1, bFile);
                        fwrite(&sfile.st_ino, sizeof(unsigned long), 1, bFile); 
                    }
                }
            }
            closedir(fd_dir); //close the directory
        }
        closedir(dir); //close the directory
    }

    if(txtFile){ //write into the compositeTablt.txt
        fprintf(txtFile, "         ===============================================================================\n");
        fclose(txtFile);
    }
    
    if(bFile){ //write into the compositeTable.bin
        char line[] = "          ================================================================================\n";
        fwrite(line, sizeof(line), 1, bFile);
        fclose(bFile);
    }

    printf("         =================================================================================\n");
}