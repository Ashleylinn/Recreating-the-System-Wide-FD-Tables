#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "functions.h"

void Vnode_table(int PID){
    ///_|> descry: this function will only display the Vnode FD table 
    ///_|> PID: given PID if speicfied, if no specified then it is -1
    ///_|> returning: this function will not return anything

    DIR *dir; 
    struct dirent *read_dir;

    printf("%-8s %-10s\n", "FD", "Inode");
    printf("=====================================\n");

    //if there is a specified PID 
    if(PID > 0){
        char path[4096];
        snprintf(path, sizeof(path), "/proc/%d/fd", PID); 
        dir = opendir(path);
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }
        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //pass if is not a number
            }
            
            char fdlink[4096];
            snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", PID, read_dir->d_name);

            int fd = atoi(read_dir->d_name);  
            struct stat sfile; 
            if(stat(fdlink, &sfile) == 0) {
                printf("%-8d %-10lu\n", fd, sfile.st_ino);

            }
        }
        closedir(dir);
    
    }else{ //if PID = -1 meaning there is no specified PID 
        dir = opendir("/proc");
        if(dir == NULL){
            perror("Cannot open the directory");
            return;
        }

        while((read_dir = readdir(dir)) != NULL){
            if(read_dir->d_name[0] < '0' || read_dir->d_name[0] > '9'){
                continue; //passs if not a number
            }

            int pid = atoi(read_dir->d_name);  //convert to integer
            char fd_path[4096]; 
            snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", pid);

            DIR *fdir = opendir(fd_path);
            if(fdir == NULL){
                continue; 
            }

            struct dirent *read_fd;
            while((read_fd = readdir(fdir)) != NULL){
                if(read_fd->d_name[0] < '0' || read_fd->d_name[0] > '9'){
                    continue;
                }

                char fdlink[4096];
                snprintf(fdlink, sizeof(fdlink), "/proc/%d/fd/%s", pid, read_fd->d_name);

                int fd = atoi(read_fd->d_name);
                struct stat sfile; 
                if(stat(fdlink, &sfile) == 0) {
                    printf("%-8d %-10lu\n", fd, sfile.st_ino);
                }
            }
            closedir(fdir);
        }
        closedir(dir);
    }
    printf("=====================================\n");
}





