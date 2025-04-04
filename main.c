#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "functions.h"

int isPID(char *arg){
    //////_|> descry: this function will check for if there is specifid PID in the argument
    ///_|> a: a is a pointer pointing to the argv[i]
    ///_|> returning: this function will return 1 if there is specified PID, otherwise 0

    for(int i = 0; arg[i] != '\0'; i++){ 
        if(arg[i] < '0' || arg[i] > '9'){
            return 0; //will return 0 if is not a numeric character
        }
    }
    return 1;
}

int main(int argc, char **argv){
    int perProcess = 0;
    int sysWide = 0;
    int Vnode = 0;
    int compos = 0;
    int summary = 0;
    int threshold = -1;
    int PID = -1;
    int TXT = 0;
    int binary = 0;

    for(int i = 1; i < argc; i++){
        if(isPID(argv[i])){ //return 1 if there is specified PID, toherwise 0
            PID = atoi(argv[i]); 
        }else if(strcmp(argv[i], "--per-process") == 0){
            perProcess = 1;
        }else if(strcmp(argv[i], "--systemWide") == 0){
            sysWide = 1;
        }else if(strcmp(argv[i], "--Vnodes") == 0){
            Vnode = 1;
        }else if(strcmp(argv[i], "--composite") == 0){
            compos = 1;
        }else if(strcmp(argv[i], "--summary") == 0){
            summary = 1;
        }else if(strncmp(argv[i], "--threshold=", 12) == 0){
            threshold = atoi(argv[i] + 12);
        }else if(strcmp(argv[i], "--output_TXT") == 0){
            TXT = 1;
        }else if(strcmp(argv[i], "--output_binary") == 0){
            binary = 1;
        }
    }    

    //default behavior
    //if no arguments then will display the composed table
    if(PID == -1 && !(perProcess || sysWide || Vnode || compos || summary || threshold > 0 || TXT || binary)){ 
        compos = 1; 
    }

    //check the threshold input is larger than 0 and displya the FD larger than threshold if provide PID
    //if no specified PID then will just show all the FD larger than that threshold
    if(threshold > 0){ 
        threshold_flag(threshold);
    }

    //specified PID with flags (ex, ./showFDtables 11873 --per-process)
    if(PID > 0 && (perProcess || sysWide || Vnode || compos || TXT || binary)){
        if(perProcess){
            perProcess_table(PID);
        }
        if(sysWide){
            sysWide_table(PID);
        }
        if(Vnode){
            Vnode_table(PID);
        }
        if(compos || TXT || binary){
            composite_table(PID, TXT, binary);
        }
        return 0;
    }

    //only specified PID but no flags (ex, ./showFDtables 11873)
    if(PID > 0){
        perProcess_table(PID);
        sysWide_table(PID);
        Vnode_table(PID);
        composite_table(PID, TXT, binary);
        return 0;
    }

    //if there is particular flags (ex, ./showFDtables --per-process)
    if(perProcess){
        perProcess_table(PID);
    }
    if(sysWide){
        sysWide_table(PID);
    }
    if(Vnode){
        Vnode_table(PID);
    }
    if(compos || TXT || binary){
        composite_table(PID, TXT, binary);
    }
    if(summary){ 
        summary_table();
    }
    
    return 0;
}