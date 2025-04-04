#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "functions.h"

void sysWide_table(int PID){
    ///_|> descry: this function will only display the system-wide FD table 
    ///_|> PID: given PID if speicfied, if no specified then it is -1
    ///_|> returning: this function will not return anything

    DIR *dir;
    struct dirent *read_dir;

    printf("%-8s %-8s %-50s\n", "PID", "FD", "Filename");
    printf("===============================================\n");

    //if there is a specified PID 
    if(PID > 0){
        char path[4096]; 
        snprintf(path, sizeof(path), "/proc/%d/fd", PID); //set path to the process file descriptors
        dir = opendir(path);
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }

        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if is not a numeric character
            }

            char fdlink[4096]; //create a string to get filenmae
            snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", PID, read_dir->d_name);

            int FD = atoi(read_dir->d_name); //convert the FD names to integers
            char fname[4096]; 
            ssize_t len = readlink(fdlink, fname, sizeof(fname) - 1); //readlink place the info of symbolic link to the fname 
            if(len != -1){
                fname[len] = '\0'; //add null terminator to fname
                printf("%-8d %-8d %-50s\n", PID, FD, fname);
            }
        }
        closedir(dir);

    }else { //if PID = -1 meaning there is no specified PID 
        dir = opendir("/proc");
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }
        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue;  //pass if is not a numeric character
            }

            int pid = atoi(read_dir->d_name);
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);

            DIR *fd_dir = opendir(path);
            if(fd_dir == NULL){
                continue; //pass if cannot open the directory
            }

            struct dirent *read_fd;
            while((read_fd = readdir(fd_dir)) != NULL){
                if(read_fd->d_name[0] < '0' || read_fd->d_name[0] > '9'){
                    continue; //pass if is not a numeric character
                }

                char fdlink[4096]; //create a string to get filenmae
                snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", pid, read_fd->d_name);

                int FD = atoi(read_fd->d_name); //convert the FD names to integer
                char fname[4096]; 
                ssize_t len = readlink(fdlink, fname, sizeof(fname) - 1); //readlink place the info of symbolic link to the fname 
                if(len != -1){
                    fname[len] = '\0'; //add null terminator to fname
                    printf("%-8d %-8d %-50s\n", pid, FD, fname);
                }   
            }
            closedir(fd_dir);
        }
        closedir(dir);
    }
    printf("===============================================\n");
}