## A Simple Linux Shell

*This is a simple command-line shell that is implemented in C as part of a lab 1 project for an Operating Systems class. This can be run in linux computers. The implemented shell allows the user to start programs and perform several simple file manipulation tasks. Using the shell, we can execute internal commands like creating and writing into a file, deleting file, copying between files, clearing the window and exit from the shell.*

#### <ins>Compilation and Execution</ins>
To compile and run the command-line shell, follow the steps below:

- You have to be on the linux/Unix terminal. Make sure you have installed necessary compilers for running C program.
- To compile the shell, navigate to the directory called "suman-os1" and run this command on the terminal:
	- To execute the shell with specified processes limit of 100, run 			```make run```
	- At the end, to clean up the compiled executable, run ```make clean```
 - With the make command, the source file will be compiled into executable and start running the implemented simple command-line shell.
		
#### <ins>Usage</ins>
Once the shell is running, you can use the following commands to perform different actions:

* **M textFile.txt** : This creates a new text file using the nano editor.
* **C existing.txt textFile.txt** : This copies the contents of existing file to textFile.
* **D existing.txt** : This deletes the existing file.
* **E (this is a comment)** : This echoes the given comment to the screen.
* **H** : This displays the user manual and helpful commands.
* **L** : This lists files in the current directory.
* **P textFile.txt** : This prints the contents of the textFile into the screen.
* **W** : This clears the screen.
* **X </program/>** : This executes the specified program. Given an executabl file called hello_world.out, we can use "X ./hello_world" to get the result.
* **Q** : This quits the shell. 

#### <ins>List of Functions the Shell Contains</ins>
The shell includes the following functions:
```c
/* Function prototypes */
// parses the command line input into command name and arguments
int parseCommand(char*, struct command_t*);

// displays the shell prompt
void printPrompt();

// reads user command from command line
void readCommand(char*);

// handles execution of internal commands like "C, D.."
void handle_internal_commands(struct command_t* command);

// helper functions for different internal commands ( C, D, E, L, H, P, M, W, X)
void copy_file(char* source, char* destination);
void delete_file(char* file);
void echo_command(char** args, int argc);
void list_directory();
void display_help();
void print_file(char* file);
void make_file(char* file);
void clear_screen();
void execute_program(char* program, char** args);
```
