# Recreating the System-Wide FD Tables

This program will display the tables used by the OS which generate the process table, system-wide FD table, Vnodes FD table, a composed view of the preveious table, and the table summarizing the number of FDs open per processes. It will be worked in the Linux OS (IA3170).

## How I solve the problem

I solve this problem by breaking all the command line arguments to each source code so that I can solve them one by one. Since splitting all the function to a single source code makes me feel easier to know what I am doing. And I first start with the composite table (composite.c) as it has all the elements (PID, FD, Filename, and Inode). So, when I do process FD table, system-wide FD table, and Vnodes table, I just modify some parts of it, which makes it easier for me. And I take the argument of PID so that i know if there is specified PID or not. If yes, then it will open up that particular PID from /proc to get all the FDs. Otherwise, it will read all directories in /proc and then get PIDs and FDs. For the summary and threshold table, they are just having the different format with the table, so I modify a bit. The threshold should return the processes with FD if the fd is larger than the specified threshold. When i run the program, I acknowlege that if I dont keep track the first process, then it will not show the PID(FD) properly, so I add a variable to keep track that only add the comma after the process. 

## Implementation  
<code>"functions.h"</code>
  * create my own header file that stores all of my function declarations
  * connect all of my source code together 
    
<code>/proc</code>
  * Get information for each processes and FDs
  * Get the FDs for specified PID in /proc/PID/fd

<code><dirent.h></code>
   * Used for the structure of dirent
   * Get the name of entry

<code><sys/types.h></code>
   * ssize_t is defined in here
   * readlink() returns the number of bytes and if not success it will return -1 as error

<code><sys/stat.h></code>
   * Used for the structure of stat
   * Get the inode

### Functions Documentation

<code>void composite_table(int PID)</code>
  * This function will only display the composed FD table (Index, PID, FD, Filenmae, and Inodes)
  * If there is a specified PID, then it will use that PID to retrieve informations
  * If there is no specified PID, then it will retrieve information from all PIDs
  * Using stat to get inode
  * Also handle arguments of --output_TXT or --output_binary to write the composed table to compositionTable.txt/compositionTable.bin

<code>void perProcess_table(int PID)</code>
 * This function will only display the process FD table (PID and FD)
 * If there is a specified PID, then it will use that PID to retrieve informations
 * If there is no specified PID, then it will retrieve information from all PIDs

<code>void sysWide_table(int PID)</code>
 * This function will display only the system-wide FD table (PID, FD, and Filenmae)
 * If there is a specified PID, then it will use that PID to retrieve informations
 * If there is no specified PID, then it will retrieve information from all PIDs

<code>void Vnode_table(int PID)</code>
 * This function display only the FD tables with Inodes
 * If there is a specified PID, then it will use that PID to retrieve informations
 * If there is no specified PID, then it will retrieve information from all PIDs
 * Using stat to get inode

<code>void summary_table()</code>
 * This function display the summarize table with PID (FD)
 * It will display all the processes and the FDs open per process

<code>void threshold_flag(int threshold)</code>
 * This function display the processs that are larger than the value X provided.
 * threshold should be larger than 0 

<code>int isPID(char *arg)</code>
 * This function checks for if the user has specified a PID
 * return 1 if user inpu a PID
   
<code>int main(int argc, char **argv)</code>
 * The main function handles all the command line arguments
 * Calls other function to display a specific FD table 


## Pseudo-code
* Parse the command line arguments:
  -  Check if there is a specified PID and is a numeric character
  -  And check if there is other flags (--composite, --per-process, --systemWide, --Vnodes, --summary, --threshold, --output_TXT, --output_binary)
* If there is no command line arguments 
  - show the default behavior of setting compos = 1
* If there is specified threshold flag
  - if is greater than 0 call threshold_flag(threshold)
* If there is specified PID
  - If is with other flags 
     - only call the tables that being selected 
  - If no other flags 
     - display all the tables (composite, per-process, systemWide, and Vnodes)
* If no specified PID (PID = -1)
  - call the tables according to the flags
    - if perProcess = 1 then will call the perProcess_table(PID)
    - if sysWide = 1 then will call the systemWide_table(PID)
    - if Vnode = 1 then will call Vnode_table(PID)
    - if compos or TXT or binary equals to 1 then will call composite_table(PID, TXT, binary)
    - if summary = 1 then will call summary_table()


## How to run the program

1. Compile with Makefile

        make
  * I use -D_GNU_SOURCE in CC which close the warning for readlink() 
  
2. Run
   
        ./showFDtables or 
        ./showFDtables with PID / flags
       (ex, ./showFDtables 11873 --composite, ./showFDtables --per-process)

3. Clean 

        make clean  

  * will remove all the object files, txt, and bin.
 
<br> 

## CLAs

* <code>--per-process</code>
  * Display only the process FD table
  * PID and FD
* <code>--systemWide</code>
  * Display only the system-wide FD table
  * PID, FD, and Filename
* <code>--Vnodes</code>
  * Display only the Vnodes FD table
  * FD and Inodes
* <code>--composite</code>
  * Display only the composed table
  * Index, PID, FD, Filenmae, and Inodes
  * Default if there is no specified argument (ex, ./showFDtables) 
* <code>--summary</code>
  * Display the table that summarize the number of FDs open per process
* <code>--threshold=X</code>
  * X is an integer
  * Processes that have a number of FD larger than X will be flagged in the output
* <code>--output_TXT</code>
  * Save the composite table in text to the file "compositeTable.txt"
  * will also print out the composed table 
* <code>--output_binary</code>
  * Save the composite table in binary format to "compositeTable.bin"
  * will also print out the composed table 

<br>

## Test cases
- <code>./showFDtables</code>
  - it shows the default behavior which is the composed table
  - if having typo for command line arguments, it will just do the default behavior which shows the composite table
- <code>./showFDtables 1134030</code>
  -  it shows all the tables with the PID of 1134030
  -  if PID not exists then it will not show any table which only the titles (PID, FD, Filenmae, Inode) and lines (=====) and will display the message of "Cannot open the directory: No such file or directory"
- <code>./showFDtables --per-process --composite</code>
   - it just shows the per process FD table and the composed table
- <code>./showFDtables --per-process --systemWide -- Vnodes --composite</code>
  - it will shows the tables with this following order
- <code>./showFDtables --composite --per-process</code>
  - it will show the perProcess table first then the composite table
  - All the table is displaying by this order (perProcess, systemWide, Vnodes, Composite)
- <code>./showFDtables --summary</code>
  - it shows the summarized table of all the processes and FDs with the format PID(FD) 
- <code>./showFDtables --threshold=20</code>
  - it will shows all the process that have FDs larger than 20
- <code>./showFDtables --composite --threshold=15</code>
  - it shows offending processes that has FD larger than 15 then display the composed table 
- <code>./showFDtables --threshold=100000000</code>
  - if there is a specifeid threshold that no FDs are larger then it will shows "No offending"
- <code>./showFDtables --output_TXT</code>
  - it will display the composed table
  - it will write the composed table to the compositeTable.txt
  - if it does not successfully open then it will send an error "Cannot open the file"
- <code>./showFDtables --output_binary</code>
  - it will display the composed table 
  - it will write the composed table to the compostieTable.bin
  - if it does not successfully open then it will send an error "Cannot open the file"

## Example
<code>./showFDtables</code>

(it will show the composed table since there is no argument)
![Image](https://drive.google.com/uc?export=view&id=1dx_Bf8Kp9SKGeSkXLBc8f-yWZb8Foed6)
