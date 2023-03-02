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
//bool pipe;
char *file;

//Prototypes
int parse_command(char command[], char *args[]);
void removeAmp_NewL(char *arg, char* args[], int& num_args);

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];          // the command that was entered
    char *args[MAX_LINE / 2 + 1];    // parsed out command line arguments
    char prev[MAX_LINE] = "\n";      // previous command history
    int should_run = 1;              // flag to determine when to exit program
    int proc, fd;                    // condition for child/parent process and file descriptors
    int num_args, bang, empty, quit; // # of args, user input '!!', user input '\n', or user input 'quit'
    //int pipe_fd[2];                  // condition for pipe process

    while (should_run)
    {
        in = false; 
        out = false;
        amp = false;
        //pipe = false;

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
                cout << prev;
                num_args = parse_command(prev, args);
            }
            else if(bang == 0 && empty == 0)
            {
                cout << "No command history found" << endl;
            }
            else
            {
                num_args = parse_command(command, args); // Parse the input command
            }

            //cout << "working, num_args: " << num_args << endl;
            if(num_args != 0)
            {

                proc = fork();

                if(proc < 0) //Fork failed
                    cout << "Fork Failed." << endl;
                else if(proc == 0) //Child process
                {
                    if(out)
                    {
                        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    else if(in)
                    {
                        fd = open(file, O_RDONLY);
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                    // else if(pipe)
                    // {

                    // }
                    
                    execvp(args[0], args);

                    cout << "Command not found" << endl;
  
                }
                else //Parent process
                {
                    //cout << "working in parent" << endl;
                    
                    if(!amp)
                        wait(NULL);
                }
            }

        }
        
    }
    return 0;
}

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

    if(strncmp(command, "\n", 1) != 0)
    {
        args[num_args++] = strtok(command, " \n");

        removeAmp_NewL(args[num_args - 1], args, num_args);

        arg = strtok(NULL, " $\n");

        //cout << "arg before while: " << arg << endl;

        while(arg != NULL)
        {
            if(strncmp(arg, ">", 1) == 0)
                out = true;
            else if(strncmp(arg, "<", 1) == 0)
                in = true;
            // else if(strncmp(arg, "|", 1) == 0)
            //     pipe = true;
            else if(in || out)
                file = arg;
            else
            {
                args[num_args++] = arg;
                removeAmp_NewL(args[num_args - 1], args, num_args);
            }
            
            arg = strtok(NULL, " $\n");
        }

        args[num_args++] = arg; //NULL
    }

    return num_args;
}

// TODO: Add additional functions if you need

void removeAmp_NewL(char* arg, char* args[], int& num_args)
{
    int size = strlen(arg);

    if(arg[size - 2] == '&' || arg[size - 1] == '&')
    {
        amp = true;
        if(size == 2)
        {
            arg[size - 2] = '\0';
            num_args--;
        }
        else if(size == 1)
        {
            arg[size - 1] = '\0';
            num_args--;
        }
        else
            arg[size - 1] = '\0';
        
        cout << "Removing amp and newline" << endl;
        cout << "arg: " << arg << endl;
        cout << "args[0]: " << args[0] << endl;
        cout << "args[1]" << args[1] << endl;
    } 
    
    if(arg[size - 1] == '\n')
    {
        cout << "arg before removing newline: " << arg << endl;
        arg[size - 1]  = '\0';
        cout << "Removing new line" << endl;
        cout << "arg: " << arg << endl;
    }
}