#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "bitmap.h"


#define ERROR_MESSAGE "Warning: one or more filter had an error, so the output image may not be correct.\n"
#define SUCCESS_MESSAGE "Image transformed successfully!\n"


/*
 * Check whether the given command is a valid image filter, and if so,
 * run the process.
 *
 * We've given you this function to illustrate the expected command-line
 * arguments for image_filter. No further error-checking is required for
 * the child processes.
 */
void run_command(const char *cmd) {
    if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "./copy") == 0 ||
        strcmp(cmd, "greyscale") == 0 || strcmp(cmd, "./greyscale") == 0 ||
        strcmp(cmd, "gaussian_blur") == 0 || strcmp(cmd, "./gaussian_blur") == 0 ||
        strcmp(cmd, "edge_detection") == 0 || strcmp(cmd, "./edge_detection") == 0) {
        execl(cmd, cmd, NULL);
    } else if (strncmp(cmd, "scale", 5) == 0) {
        // Note: the numeric argument starts at cmd[6]
        execl("scale", "scale", cmd + 6, NULL);
    } else if (strncmp(cmd, "./scale", 7) == 0) {
        // Note: the numeric argument starts at cmd[8]
        execl("./scale", "./scale", cmd + 8, NULL);
    } else {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        exit(1);
    }
}


// TODO: Complete this function.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: image_filter input output [filter ...]\n");
        exit(1);
    }
    
    // If only 3 arguements are provided, just copy image to stdout
    else if(argc == 3) {
        
        // Create a pipe
        int fd[2];  //array of two file descriptors (for reading and writing)
        pipe(fd);
        
        int file1 = open(argv[1], O_RDWR);
        int file2 = open(argv[2], O_RDWR);
        
        int r = fork();
        
        if(r == 0) { //ie. child
            dup2(file1, fileno(stdin));     // redirect the standard input to file1
            dup2(file2, fileno(stdout));    //redirect standard outout to file2
            close(fd[0]);
            close(fd[1]);
            
            run_command("./copy");
            exit(0);
            
            
        }
        else if(r > 0) { //ie parent
            close(fd[0]);
            close(fd[1]);
            
            int exit_status;
            int status;
            if(wait(&status) != -1) {
                if (WIFEXITED(status)) {
                    exit_status = WEXITSTATUS(status);
                    if (exit_status == 0) {
                        printf(SUCCESS_MESSAGE);
                    }
                    else {
                        printf(ERROR_MESSAGE);
                    }
                }
            }
            
        }
    }
    
    
    // else we need to loop through all the arguements provided and apply filter
    else {
        
        for(int i = 3; i < argc + 1; i++) {
            int file1 = open(argv[1], O_RDWR);
            int file2 = open(argv[2], O_RDWR);
            int fds[2];  //array of two file descriptors (for reading and writing)
            pipe(fds);
            int r = fork();
            if(r == 0) { //ie. child
                dup2(file1, fileno(stdin));     // redirect the standard input to file1
                dup2(file2, fileno(stdout));    //redirect standard outout to file2
                close(fds[0]);
                close(fds[1]);
                
                char *filter = argv[i];
                run_command(filter);
                
                
            }
            else if(r > 0) { //ie parent
                close(fds[0]);
                close(fds[1]);
                
                int exit_status;
                int status;
                if(wait(&status) != -1) {
                    if (WIFEXITED(status)) {
                        exit_status = WEXITSTATUS(status);
                        if (exit_status == 0) {
                            printf(SUCCESS_MESSAGE);
                            exit(0);
                        }
                        else {
                            printf(ERROR_MESSAGE);
                            exit(1);
                        }
                    }
                }
                
            }
            
            
        }
    }
    return 0;
}
