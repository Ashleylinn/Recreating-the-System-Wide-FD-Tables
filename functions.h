#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void composite_table(int PID, int TXT, int binary);
void perProcess_table(int PID);
void sysWide_table(int PID);
void Vnode_table(int PID);
void summary_table();
void threshold_flag(int threshold);

#endif
