#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"

void summary_table(){
    ///_|> descry: this function will display the summarize table with the format like this PID(FD)
    ///_|> returning: this function will not return anything

    DIR *dir = opendir("/proc"); 
    if(dir == NULL){
        perror("Cannot open directory");
        return;
    }

    printf("      Summary Table\n");
    printf("    ===================\n");
    
    struct dirent *read_dir; //create a pointer pointing to a structure of dirent
    int firstPro = 1; //keep track if it is the first PID to ensure no extra comma before the first process
    while((read_dir = readdir(dir)) != NULL){
        if(read_dir->d_name[0] >= '0' && read_dir->d_name[0] <= '9'){  //check if is a numerica character 
            int pid = atoi(read_dir->d_name); 
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);

            DIR *fdir = opendir(path);
            if(fdir == NULL){
                continue; //pass when the directory cannot open
            }

            int num_fd = 0; //kepp track for the number of FDs for each process 
            struct dirent *read_fd;
            while((read_fd = readdir(fdir)) != NULL){ 
                if(read_fd->d_name[0] >= '0' && read_fd->d_name[0] <= '9'){
                    num_fd++; //counting the fd of processes if is the numeric character
                }
            }
            closedir(fdir);
            
            if(!firstPro){ //add comma after the first proceses
                printf(", ");
            }
            printf("%d (%d), ", pid, num_fd);
            firstPro = 0; //reset to zero after the first process has been printed

        }
    }
    closedir(dir);
    printf("\n");
}