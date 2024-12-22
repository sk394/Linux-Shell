//Simple Linux shell

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>


#define MAX_ARGS		64   
#define MAX_ARG_LEN		16   // Maximum length for each argument
#define MAX_LINE_LEN	80   // Maximum length for a command line input
#define WHITESPACE		" ,\t\n"

 /// <summary>
 /// This struct is used to store the command name, number of arguments and the array of arguments
 /// </summary>
struct command_t {
	char* name;
	int argc;
	char* argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char*, struct command_t*);
void printPrompt();
void readCommand(char*);
void handle_internal_commands(struct command_t* command);
void copy_file(char* source, char* destination);
void delete_file(char* file);
void echo_command(char** args, int argc);
void list_directory();
void display_help();
void print_file(char* file);
void make_file(char* file);
void clear_screen();
void execute_program(char* program, char** args);

int main(int argc, char* argv[]) {
	int pid;
	int status;
	char cmdLine[MAX_LINE_LEN];
	struct command_t command;

	/*
	* It continuously runs the shell until the user enters 'Q' to quit the shell. It reads the command line, parses it and executes the commands.
	*/
	while (1) {
		printPrompt();
		/* Read the command line and parse it */
		readCommand(cmdLine);
		parseCommand(cmdLine, &command);
		command.argv[command.argc] = NULL; // null-terminate the argument list

		//Check if internal command is Q (quit)
		if (strcmp(command.name, "Q") == 0) {
			break; // Quit the shell
		}

		// check if the command is an internal command
		if (strcmp(command.name, "C") == 0 || strcmp(command.name, "D") == 0 ||
			strcmp(command.name, "E") == 0 || strcmp(command.name, "H") == 0 ||
			strcmp(command.name, "L") == 0 || strcmp(command.name, "M") == 0 ||
			strcmp(command.name, "P") == 0 || strcmp(command.name, "W") == 0 || strcmp(command.name, "X") == 0) {

			// handle internal commands
			handle_internal_commands(&command);
		}
		else {
			// if it is an external command, fork and execvp it
			pid = fork();
			if (pid == 0) { // child process
				execvp(command.name, command.argv); // execute the command for the child process
				perror("execvp"); // print error if execvp fails
				exit(1);
			}
			else if (pid > 0) { // parent process
				waitpid(pid, &status, 0); // wait for the child process to finish
			}
			else {
				// if fork fails
				perror("fork");
				exit(1);
			}
		}
	}

	/* Shell termination */
	printf("\n\n shell: Terminating successfully\n");
	return 0;
}

/* Parse Command function */
/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char* cLine, struct command_t* cmd) {
	int argc;
	char** clPtr;
	/* Initialization */
	clPtr = &cLine;	/* cLine is the command line */
	argc = 0;
	cmd->argv[argc] = (char*)malloc(MAX_ARG_LEN);
	/* Fill argv[] */
	while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
		cmd->argv[++argc] = (char*)malloc(MAX_ARG_LEN);
	}

	/* Set the command name and argc */
	cmd->argc = argc - 1;
	cmd->name = (char*)malloc(sizeof(cmd->argv[0]));
	strcpy(cmd->name, cmd->argv[0]);
	return 1;
}

/* Print prompt and read command functions - Nutt pp. 79-80 */
void printPrompt() {
	/* Build the prompt string to have the machine name,
	 * current directory, or other desired information
	 */
	printf("linux (sk394)|> ");
}

/* Read the command line and copy it to the buffer */
void readCommand(char* buffer) {
	/* This code uses any set of I/O functions, such as those in
	 * the stdio library to read the entire command line into
	 * the buffer. This implementation is greatly simplified,
	 * but it does the job.
	 */
	fgets(buffer, 80, stdin);
	buffer[strcspn(buffer, "\n")] == '\0';
}


/**
 * It executes the internal commands of the shell. This function is called when the command is one of the internal commands.
 * @param command: Pointer to the command struct that contains the parsed command and arguments
 */
void handle_internal_commands(struct command_t* command) {
	// if the command name is "C", call the copy_file function
	if (strcmp(command->name, "C") == 0) {
		if (command->argc >= 2) { // for copying, we need at least two arguments
			copy_file(command->argv[1], command->argv[2]);
		}
		else {
			fprintf(stderr, "Usage: C file1 file2\n");
		}
	}
	// if the command name is "D", call the delete_file function
	else if (strcmp(command->name, "D") == 0) {
		if (command->argc >= 1) { // for deleting, we need at least one argument
			delete_file(command->argv[1]);
		}
		else {
			fprintf(stderr, "Usage: D file\n");
		}
	}
	// if the command name is "E", call the echo_command function
	else if (strcmp(command->name, "E") == 0) {
		echo_command(command->argv, command->argc);
	}
	// if the command is "L", call the list_directory function
	else if (strcmp(command->name, "L") == 0) {
		list_directory();
	}
	// if the command is "H", call the display_help function
	else if (strcmp(command->name, "H") == 0) {
		display_help();
	}
	// if the command is "M", call the make_file function
	else if (strcmp(command->name, "M") == 0) {
		if (command->argc >= 1) { // for creating a file, we need at least one argument
			make_file(command->argv[1]);
		}
		else {
			fprintf(stderr, "Usage: M file\n");
		}
	}
	// if the command is "P", call the print_file function
	else if (strcmp(command->name, "P") == 0) {
		if (command->argc >= 1) { // needs at least one argument
			print_file(command->argv[1]);
		}
		else {
			fprintf(stderr, "Usage: P file\n");
		}
	}
	// if the command is "W", call the clear_screen function
	else if (strcmp(command->name, "W") == 0) {
		clear_screen();
	}
	// if the command is "X", call the execute_program function
	else if (strcmp(command->name, "X") == 0) {
		if (command->argc >= 2) { // needs at least 2 arguments
			execute_program(command->argv[1], &command->argv[1]);
		}
		else {
			fprintf(stderr, "Usage: X program\n");
		}
	}
}

/*
This function copies the source file to the destination file. This functions forks a child process to execute the "cp" command.
The child process then replaces its execution with the "cp" command to copy the file.
*/
void copy_file(char* source, char* destination) {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "cp", source, destination, NULL };
		execvp("cp", args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL);
	}
};

/*
This function forks a child process to execute the "rm" command to delete the file.
*/
void delete_file(char* file) {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "rm", file, NULL };
		execvp("rm", args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL);
	}
}

/* This function prints the comments to the screen. If no argument is passed, a new prompt is issued. */
void echo_command(char** args, int argc) {
	for (int i = 1; i < argc; i++) {
		printf("%s ", args[i]);
	}
	printf("\n");
}

/* This function lists the working directory and its contents. It forks a child process to execute the "pwd" command to print the working directory.
After "pwd" completes, the child process executes the "ls -l" command to list the contents of the directory.
*/
void list_directory() {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "pwd", NULL };
		execvp("pwd", args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL); // wait for the "pwd" child process to complete
		pid_t pid_ls = fork();
		if (pid_ls == 0) {
			char* args[] = { "ls", "-l", NULL };
			execvp("ls", args);
			perror("execvp");
			exit(EXIT_FAILURE);
		}
		else {
			wait(NULL);
		}
	}
}

/*
This function will fork a child process and execute the more command in the child to display the contents of the file. The child process then replaces its
execution with the `more` command to print the contents of the specified file to the terminal.
*/
void print_file(char* file) {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "more", file, NULL };
		execvp("more", args); // replace the child process with the "more" command
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL); // wait for the child process to finish
	}
};

/*This function will fork a child process and executes the nano command in the child for creating a file with nano editor, and wait for it to finish in the parent.*/
void make_file(char* file) {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "nano", file, NULL };
		execvp("nano", args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL);
	}
};

/*This function will fork a child process and executes the clear command in the child, and wait for it to finish in the parent.*/
void clear_screen() {
	pid_t pid = fork();
	if (pid == 0) {
		char* args[] = { "clear", NULL };
		execvp("clear", args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL);
	}
};

/*
* Execute a program with execvp. This function will fork a child process and executes the program in the child, and wait for it to finish in the parent.
* @param program : The name of the program to be executed ( X ./output)
* @param args: The array of arguments to be passed to the program
*/
void execute_program(char* program, char** args) {
	pid_t pid = fork();
	if (pid == 0) {
		execvp(program, args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else {
		wait(NULL);
	}
}

// This displays a simple manual describing on how to use this shell and its commands on the screen.
void display_help() {
	printf("--------------------A Simple Shell Manual-------------------------\n\n");
	printf("NAME\n");
	printf("       Simple Shell - A simple command line shell written using\n");
	printf("       standard C functions and POSIX system calls\n\n");
	printf("SYNOPSIS\n");
	printf("       [options] [command_string|file]\n\n");
	printf("DESCRIPTION\n");
	printf("       Simple Shell is a minimalist command-line shell that runs on linux comp\n");
	printf("       -uter. It allows the user to start programs and perform several simple\n");
	printf("       file manipulation tasks. User can type the command and arguments and execute\n");
	printf("       the command by entering 'Enter' key. Check the following commands for more info:\n\n");
	printf("COMMANDS\n");
	printf("       C file1 file2\n");
	printf("              Copy the contents of file1 to file2. If file2 doesn't exist, it\n");
	printf("              will be created. If file2 already exists, it will be overwritten.\n\n");
	printf("       D file\n");
	printf("              Delete the specified file.\n\n");
	printf("       E comment\n");
	printf("              Echo the comment to the screen.\n\n");
	printf("       H\n");
	printf("              Display this user manual.\n\n");
	printf("       L\n");
	printf("              List the contents of the current directory.\n\n");
	printf("       M file\n");
	printf("              Create a new text file using the nano editor.\n\n");
	printf("       P file\n");
	printf("              Print the contents of the specified file using the more command.\n\n");
	printf("       W\n");
	printf("              Clear the screen, providing a clean interface to write commands.\n\n");
	printf("       X program\n");
	printf("              Execute the named program. Eg. you can use 'X /.name'\n");
	printf("       Q\n");
	printf("              Quit the shell and return to the parent process.\n\n");
	printf("EXTERNAL COMMANDS\n");
	printf("       Any command not listed above will be treated as an external command and\n");
	printf("       executed using the execvp() system call. This allows you to run standard\n");
	printf("       Unix/Linux commands and other installed programs.\n\n");
	printf("       Example: mkdir ~/dirName\n\n");
}


