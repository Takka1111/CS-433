/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Abraham Gomez, Tucker Shaw
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

//Define global variables
bool amp, in, out = false;
char *file;

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    int num_args = 0;
    char *arg;

    //cout << "enter" << command << endl;

    if(strncmp(command, "\n", 1))
    {
        args[num_args++] = strtok(command, " \n");
   
        //cout << "args: " << args[0] << endl;

        arg = strtok(NULL, " \n");

        while(arg != NULL)
        {
            if(strncmp(arg, ">", 1) == 0)
            {
                out = true;
            }
            else if(strncmp(arg, "<", 1) == 0)
            {
                in = true;
            }
            else if(strncmp(arg, "&", 1) == 0)
            {
                amp = true;
            }
            else if(in || out)
            {
                file = arg;
            }
            else
            {
                args[num_args++] = arg;
            }
            
            arg = strtok(NULL, " \n");
        }

        args[num_args++] = arg;

    }

    //cout << "end" << endl;

    return num_args;
}

// TODO: Add additional functions if you need

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    int proc, w_proc;             // condition for child and parent process
    int num_args, bang, empty, quit; // number of arguments for command
    int fd;
    char prev[MAX_LINE] = "\n";

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        
        // Read the input command
        fgets(command, MAX_LINE, stdin);

        num_args = 0;

        bang = strcmp(command, "!!\n");
        empty = strncmp(prev, "\n", 1);
        quit = strcmp(command, "exit\n");

        //cout << "Before copying command: " << command << " prev: " << prev << endl;

        if(bang != 0)
            strcpy(prev, command);

        //cout << "After copying command: " << command << " prev: " << prev << endl;

        if(quit == 0)
            should_run = 0;
        else
        {
            if(bang == 0 && empty != 0)
            {
                num_args = parse_command(prev, args);
            }
            else if(bang == 0 && empty == 0)
            {
                cout << "No commands in history." << endl;
            }
            else
            {
                num_args = parse_command(command, args); // Parse the input command
            }

            //cout << "working, num_args: " << num_args << endl;
            if(num_args != 0)
            {
                if(out)
                {
                    fd = open(file, O_WRONLY, O_CREAT, O_TRUNC);
                    dup2(fd, STDOUT_FILENO);
                }
                else if(in)
                {
                    fd = open(file, O_RDONLY);
                    dup2(fd, STDIN_FILENO);
                }

                proc = fork();

                if(proc < 0) //Fork failed
                {
                    //cout << "Failed" << endl;
                    exit(1);
                }
                else if(proc == 0) //Child process
                {
                    //cout << "working in child" << endl;
                    execvp(args[0], args);
                }
                else //Parent process
                {
                    //cout << "working in parent" << endl;
                    
                    if(!amp)
                        w_proc = wait(NULL);
                    
                    if(in || out)
                        close(fd);
                }
            }

        }
       
    }
    return 0;
}
