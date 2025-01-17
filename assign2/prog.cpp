/**
 * Assignment 2: Simple UNIX Shell
 * @file prog.cpp
 * @author Abraham Gomez, Tucker Shaw
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */

// Include required libraries
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief Flags struct for condtions within commands
 * @brief Allows for I/O redirection, piping, concurrency
 */
struct
{
    bool input_F, output_F, pipe_F, ampersand_F;
    int should_run;
    char* file_name;
} Flags;

// Prototypes
void exec(char command[], char prev[], char *args[]);
int parse_command(char command[], char *args[], char* argsP[]);
void removeAmp(char *arg, char* args[], int& num_args);
void execFork(char* args[]);
void execForkPipe(char* args[], char* argsP[]);
void setFlags();

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    //Declare variables
    char command[MAX_LINE];             // the command that was entered
    char *args[MAX_LINE / 2 + 1];       // parsed out command line arguments
    char prev[MAX_LINE] = "\n";         // previous command history
    Flags.should_run = 1;               // flag to determine when to exit program

    while (Flags.should_run) // Running the Unix Shell
    {
        setFlags(); // Set flag conditions

        printf("osh>"); // Print terminal input line
        fflush(stdout); // Sets a buffer for the output
        
        fgets(command, MAX_LINE, stdin); // Read the input command

        exec(command, prev, args); // Execute main shell functions
    }

    return 0;
}

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param command Command entered by user on command line
 * @param prev    Previous command entered by user
 * @param args    Parsed out command line arguments
 * @return VOID
 */
void exec(char command[], char prev[], char *args[])
{
    // Declare variables
    int num_args = 0;               // # of args for command execution
    char *argsP[MAX_LINE / 2 + 1];  // parsed out command line arguments for RHS pipe

    // Check for user history command '!!', no command, and 'exit' command
    int history = strcmp(command, "!!\n");
    int empty = strncmp(prev, "\n", 1);
    int quit = strcmp(command, "exit\n");

    if(history != 0) // Store current command in prev
        strcpy(prev, command);

    if(quit == 0) // User entered exit, quit the shell
        Flags.should_run = 0;
    else
    {
        if(history == 0 && empty != 0) // User wants to use prev command
        {
            // Display previous command and parse
            cout << prev;
            num_args = parse_command(prev, args, argsP);
        }
        else if(history == 0 && empty == 0) // No previous history found
        {
            cout << "No command history found" << endl;
        }
        else // Parse current command
        {
            num_args = parse_command(command, args, argsP); // Parse the input command
        }

        if(num_args != 0) // Check if a command was entered
        {
            
            if(Flags.pipe_F) // If user piping, start the pipe process
                execForkPipe(args, argsP);
            else // Run regular command
                execFork(args);
        }
    }
}

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[], char *argsP[])
{
    // Declare variables
    int num_args = 0;       // #args for the array
    int num_argsP = 0;      // #args for pipe array
    char *arg;              // stores current arg being parsed from command line

    if(strncmp(command, "\n", 1) != 0) // Check if any command was entered
    {
        args[num_args++] = strtok(command, " \n"); // Parse first arg from command line

        removeAmp(args[num_args - 1], args, num_args); // Check for ampersand in arg

        arg = strtok(NULL, " $\n"); // Get next arg

        while(arg != NULL) // Check for more args in command line
        {
            if(strncmp(arg, ">", 1) == 0) // Check if output redirection, set flag
                Flags.output_F = true;
            else if(strncmp(arg, "<", 1) == 0) // Check if input redirection, set flag
                Flags.input_F = true;
            else if(strncmp(arg, "|", 1) == 0) // Check if piping, set flag
                Flags.pipe_F = true;
            else if(Flags.file_name == NULL && (Flags.input_F || Flags.output_F)) // Set file name for redirection
                Flags.file_name = arg;
            else if(Flags.pipe_F) // Check if LHS gathered
            {
                argsP[num_argsP++] = arg; // Add argument to RHS args
                removeAmp(argsP[num_argsP - 1], argsP, num_argsP); // Check for concurrency
            }
            else
            {
                args[num_args++] = arg; // Add the current arg
                removeAmp(args[num_args - 1], args, num_args); // Check for concurrency
            }

            arg = strtok(NULL, " $\n"); // Get next arg
        }

        args[num_args++] = arg; // Add NULL to args list
        argsP[num_argsP++] = arg; // Add NULL to RHS args list for pipe
    }

    return num_args; // Return new #args
}

/**
 * @brief Remove the ampersand character from parsed args
 *
 * @param arg //Argument to modify
 * @param args //Array of args
 * @param num_args //# of args
 * @return VOID
 */
void removeAmp(char *arg, char *args[], int& num_args)
{
    // Declare variables
    int size = strlen(arg);     // Get the size of the argument

    if(arg[size - 2] == '&' || arg[size - 1] == '&') // Check if ampersand in argument
    {
        Flags.ampersand_F = true; // Set flag for concurrency

        if(size == 1) // Ampersand inputted at end of command line (no args)
        {
            arg[size - 1] = '\0';
            num_args--;
        }
        else // Ampersand inputted with args
            arg[size - 1] = '\0';
    } 
}

/**
 * @brief Fork a child process for executing command line
 *
 * @param args //Array of args
 * @return void function
 */
void execFork(char *args[])
{
    // Declare variables
    int fd;             // condition for I/O redirection
    int pid;            // condition for fork

    pid = fork();

    if(pid < 0) // Fork failure
        cout << "Fork Failed." << endl;
    else if(pid == 0) // Child process executes
    {
        if(Flags.output_F) // Check for output redirection
        {
            // Redirect file descriptor for std output
            fd = open(Flags.file_name, O_CREAT | O_TRUNC | O_WRONLY);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if(Flags.input_F) // Check for input redirection
        {
            // Redirect file descriptor for std input
            fd = open(Flags.file_name, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(execvp(args[0], args) < 0) // Child process executing
            cout << "Command not found" << endl; // Invalid command was entered
    }
    else // Parent process executes
    {
        if(!Flags.ampersand_F) // Check for concurrency
            wait(NULL); // Parent waits for child
    }
}

/**
 * @brief Fork two child processes for piped commands
 *
 * @param args  // Array of args for LHS of pipe
 * @param argsP // Array of args for RHS of pipe
 * @return VOID
 */
void execForkPipe(char *args[], char *argsP[])
{
    // Declare variables
    int pid;         // condition for forks
    int fd;          // file descriptor for I/O redirection
    int pipe_fd[2];  // pipe process file descriptors

    if(pipe(pipe_fd) < 0) //Create the pipe and check for failure
        cout << "Pipe Failed" << endl;
    else
    {
        pid = fork(); // First child process

        if(pid < 0) // Fork failure
            cout << "Fork Failed" << endl;
        else if(pid == 0) // Child process executes
        {
            dup2(pipe_fd[1], STDOUT_FILENO); // Set pipe condition for output
            close(pipe_fd[0]); // Close pipe fd for reading
            close(pipe_fd[1]); // Close pipe fd for writing
            
            if(execvp(args[0], args) < 0) // LHS child process executes
                cout << "Command not found" << endl;
        }
        else // Parent process executes
        {
            pid = fork(); // Second child process

            if(pid < 0) // Fork failure
                cout << "Fork Failed" << endl;
            else if(pid == 0) // Child process executes
            {
                if(Flags.output_F) // Check for output redirection
                {
                    // Redirect file descriptor for std output
                    fd = open(Flags.file_name, O_CREAT | O_TRUNC | O_WRONLY);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else if(Flags.input_F) // Check for input redirection
                {
                    // Redirect file descriptor for std input
                    fd = open(Flags.file_name, O_RDONLY);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                dup2(pipe_fd[0], STDIN_FILENO); // Set pipe condition for reading input
                close(pipe_fd[1]); // Close pipe fd for reading
                close(pipe_fd[0]); // Close pipe fd for writing
                
                if(execvp(argsP[0], argsP) < 0) // RHS child process executes
                    cout << "Command not found" << endl;
            }
            else // Parent process executes
            {
                close(pipe_fd[0]); // Close parent pipe fd for read
                close(pipe_fd[1]); // Close parent pipe fd for write

                if(!Flags.ampersand_F)
                    wait(NULL); // Parent process waits
            }
        }
    }
}

/**
 * @brief Resets Flags struct conditions
 * @return VOID
 */
void setFlags()
{
    Flags.input_F = false;
    Flags.output_F = false;
    Flags.pipe_F = false;
    Flags.ampersand_F = false;
    Flags.file_name = NULL;
}