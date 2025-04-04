#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"

void perProcess_table(int PID){
    ///_|> descry: this function will only display the process FD table
    ///_|> PID: given PID if speicfied, if no specified then it is -1
    ///_|> returning: this function will not return anything

    DIR *dir;
    struct dirent *read_dir; 

    printf("%-8s %-8s\n", "PID", "FD"); 
    printf("========================\n");

    //if there is a specified PID 
    if(PID > 0){
        char path[4096];
        snprintf(path, sizeof(path), "/proc/%d/fd", PID); //set path to the process file descriptors
        dir = opendir(path);
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }

        while((read_dir = readdir(dir)) != NULL){ //reading each FDs
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if is not a numeric character
            }
            int FD = atoi(read_dir->d_name); //convert the FD names to integers
            printf("%-8d %-8d\n", PID, FD);
        }
        closedir(dir); //close the directory

    }else { //if PID = -1 meaning there is no specified PID 
        dir = opendir("/proc");
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }

        while((read_dir = readdir(dir)) != NULL){ //rad each process directory
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if is not a numeric character
            }
            int pid = atoi(read_dir->d_name);
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%d/fd", pid); 

            DIR *fd_dir = opendir(path); //open the FD of this process
            if(fd_dir == NULL){
                continue; //pass if the directory cannot open
            }

            struct dirent *read_fd;
            while((read_fd = readdir(fd_dir)) != NULL){ //looping through each FD
                if(read_fd->d_name[0] < '0' || read_fd->d_name[0] > '9'){
                    continue; //pass if is not numbers
                }

                int FD = atoi(read_fd->d_name); 
                if(FD >= 0){ //check FD as if is -1 then it will returns messy info
                    printf("%-8d %-8d\n", pid, FD);
                }
            }
            closedir(fd_dir);  
        }
        closedir(dir);
    }
    printf("========================\n");
}