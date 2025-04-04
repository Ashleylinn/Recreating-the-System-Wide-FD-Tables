#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"

void threshold_flag(int threshold){
    ///_|> descry: this function will list the process that has FD larger than the threshold 
    ///_|> threshold: specified X value (integer), if nothing then will be -1
    ///_|> returning: this function will not return anything

    DIR *dir = opendir("/proc"); //read from /proc
    if(dir == NULL){
        perror("Cannot open the directory");
        return;
    }
    
    printf(" ## Offending processes:\n");

    struct dirent *read_dir;
    int firstPro = 1; //keep track if it is the first PID to ensure no extra comma before the first process
    int get = 0; //create a variable to check no offending processes printed if not larger than threshold
    while((read_dir = readdir(dir)) != NULL){
        if(read_dir->d_name[0] >= '0' && read_dir->d_name[0] <= '9'){ //check if is a numerica character 
            int pid = atoi(read_dir->d_name); 
            char path[4096]; 
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);

            DIR *fdir = opendir(path);
            if(fdir == NULL){
                continue; //you pass if the directory cannot open
            }

            int num_fd = 0;
            struct dirent *read_fd;
            while((read_fd = readdir(fdir)) != NULL){
                if(read_fd->d_name[0] >= '0' && read_fd->d_name[0] <='9'){ 
                    num_fd++; //counting the fd of processes if is the numeric character
                }
            }
            closedir(fdir);

            //only print out if the FD count is larger than the threshold
            if(num_fd > threshold){ 
                if(!firstPro){ 
                    printf(", "); //add comma if is not the first pID
                }
                printf("%d (%d)", pid, num_fd); 
                firstPro = 0; //reset to zero after the first process 
                get = 1; //set to 1 to know there is one process printed
            }
        }
    }
    closedir(dir);
    if(get){ 
        printf("\n"); //if there is then create a newline
    }else{
        printf("No offending\n"); //if there is no process larger than the threshold
    }
}