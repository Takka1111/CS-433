/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Abraham Gomez, Tucker Shaw
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */

//Include required libraries
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
bool amp, in, out;
//bool pipe;
char *file;

//Prototypes
int parse_command(char command[], char *args[]);
void removeAmp(char *arg, char* args[], int& num_args);

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    //Declare variables
    char command[MAX_LINE];          // the command that was entered
    char *args[MAX_LINE / 2 + 1];    // parsed out command line arguments
    char prev[MAX_LINE] = "\n";      // previous command history
    int should_run = 1;              // flag to determine when to exit program
    int proc, fd;                    // condition for child/parent process and file descriptors
    int num_args, bang, empty, quit; // # of args, user input '!!', user input '\n', or user input 'quit'
    //int pipe_fd[2];                  // condition for pipe process

    while (should_run) //Running the Unix Shell
    {
        //Set boolean conditions to false
        in = false; 
        out = false;
        amp = false;
        //pipe = false;

        num_args = 0;

        printf("osh>"); //Print terminal input line
        fflush(stdout); //Sets a buffer for the output
        
        fgets(command, MAX_LINE, stdin); // Read the input command

        //Check for user history, no command, and exit shell
        bang = strcmp(command, "!!\n");
        empty = strncmp(prev, "\n", 1);
        quit = strcmp(command, "exit\n");

        if(bang != 0) //Store current command in prev
            strcpy(prev, command);

        if(quit == 0) //User entered exit, quit the shell
            should_run = 0;
        else
        {
            if(bang == 0 && empty != 0) //User wants to use prev command
            {
                //Display previous command and parse
                cout << prev;
                num_args = parse_command(prev, args);
            }
            else if(bang == 0 && empty == 0) //No previous history found
            {
                cout << "No command history found" << endl;
            }
            else //Parse current command
            {
                num_args = parse_command(command, args); // Parse the input command
            }

            if(num_args != 0) //Check if a command was entered
            {
                proc = fork(); //Create child process for current command

                if(proc < 0) //Fork failed
                    cout << "Fork Failed." << endl;
                else if(proc == 0) //Child process
                {
                    if(out) //Check for output redirection
                    {
                        //Redirect file descriptor for std output
                        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    else if(in) //Check for input redirection
                    {
                        //Redirect file descriptor for std input
                        fd = open(file, O_RDONLY);
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                    // else if(pipe)
                    // {
                    //     dup2(pipe_fd[0], STDOUT_FILENO);
                    //     dup2(pipe_fd[1], STDIN_FILENO);
                    // }
                    
                    execvp(args[0], args); //Child process executing

                    cout << "Command not found" << endl; //Invalid command was entered
  
                }
                else //Parent process
                {      
                    if(!amp) //Check for concurrency
                        wait(NULL); //Parent waits for child
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
    //Declare variables
    int num_args = 0; //#args for the array
    char *arg; //stores current arg being parsed from command line

    if(strncmp(command, "\n", 1) != 0) //Check if any command was entered
    {
        args[num_args++] = strtok(command, " \n"); //Parse first arg from command line

        removeAmp(args[num_args - 1], args, num_args); //Check for ampersand in arg

        arg = strtok(NULL, " $\n"); //Get next arg

        while(arg != NULL) //Check for more args in command line
        {
            if(strncmp(arg, ">", 1) == 0) //Check if output redirection, set condition
                out = true;
            else if(strncmp(arg, "<", 1) == 0) //Check if input redirection, set condition
                in = true;
            // else if(strncmp(arg, "|", 1) == 0)
            //     pipe = true;
            else if(in || out) //Set file name for redirection if true
                file = arg;
            else
            {
                args[num_args++] = arg; //Add the current arg
                removeAmp(args[num_args - 1], args, num_args); //Check for ampersand
            }

            arg = strtok(NULL, " $\n"); //Get next arg
        }

        args[num_args++] = arg; //Add NULL to args list
    }

    return num_args; //Return new #args
}

/**
 * @brief Remove the ampersand character from parsed args
 *
 * @param arg //Argument to modify
 * @param args //Array of args
 * @param num_args //# of args
 * @return void function
 */
void removeAmp(char* arg, char* args[], int& num_args)
{
    //Declare variables
    int size = strlen(arg); //Get the size of the argument

    if(arg[size - 2] == '&' || arg[size - 1] == '&') //Check if ampersand in argument
    {
        amp = true; //Set condition for concurrency

        if(size == 1) //Ampersand inputted at end of command line (no args)
        {
            arg[size - 1] = '\0';
            num_args--;
        }
        else //Ampersand inputted with args
            arg[size - 1] = '\0';
    } 
}