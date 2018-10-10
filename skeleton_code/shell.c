/******************************************************************************
*
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.c processes the users input, passes it to a tokenizer, and then passes
* the tokens obtained to the process_commands function which will process and
* execute the commands.
*
******************************************************************************/

#include "shell.h"
#include "commandParser.h"

/******************************************************************************
* Processes the input and determine whether it is a user interface operation
* or a set of commands that will need to be executed.
******************************************************************************/
void shell(char* filename)
{

	/**************************************************************************
	* short			special_char 	determines whether the character to be
	*								processed is special or not.
	* int 			len 			keeps track of the current line length
	* char 			line 			holds current line
	**************************************************************************/
	short special_char = FALSE;
	int status, len = 0;
	char ch, *line = (char*)malloc(MAX_LEN);
	FILE *fp = NULL;

	if (filename != NULL)
	{
		fp = fopen(filename, READ);

		if (fp == NULL) printf("Unable to open %s\n", filename);

	}

	/**************************************************************************
	* Loops until the user exits the program.
	**************************************************************************/
	print_user();
	while(TRUE)
	{

		ch = getch(fp);

		if (special_char)
		{
			special_char = FALSE;
			ch = switch_keypad(ch);
		}

		/**********************************************************************
		* switchs arround possible cases depending on the read character
		**********************************************************************/
		switch(ch)
		{
			/******************************************************************
			* handles the ascii translation of arrow characters
			******************************************************************/
			case '\033':
				getch(NULL);
				special_char = TRUE;
				continue;
				break;

			/******************************************************************
			* ignore arrow characters and tab
			******************************************************************/
			case KEYLEFT:
			case KEYRIGHT:
			case KEYUP:
			case KEYDOWN:
			case '\t':
				break;

			/******************************************************************
			* handles backspacing
			******************************************************************/
			case DELETE:
			{
				if (len > 0)
					delete(--len, line);
				break;
			}

			/******************************************************************
			* if the maximum line length is not exceeded the program will print
			* the character. if the character is not a new line then continue.
			* Else, terminate line, pass it to the execute_commands methond,
			* allocate a new line, and reset the line length.
			******************************************************************/
			default:
			{
				if (len < MAX_LEN)
				{

					if (ch != '\n')
					{
						printf("%c", ch);
						line[len++] = ch;
					}

					else if (ch == '\n' && len > 0)
					{
						printf("%c", ch);
						line[len] = '\0';
						status = execute_commands(line);

						free(line);
						if (status == TERMINATE) exit(SUCCESS);

						line = (char*)malloc(MAX_LEN);
						len = 0;

						print_user();
					}
				}
				break;
			}
		}
	}

	fclose(fp);
}

/******************************************************************************
* execute_commands will process and execute the commands in the variable line.
******************************************************************************/
short execute_commands(char* line) //compile with c99
{
	short status;
	//Your code goes here.

	char *pipeFilterArray[MAX_LEN]; //each index contains the commmands separated by the pipe filter
	char *parsed[MAX_LEN];//each index contains each command separated by spaces
	//end of each command is marked by "/0"
	char *commands[MAX_LEN];//copy of pipe filter array
	int i = 0; //counter used to keep track of the commands for parsing spaces
	int j = 0;
	// printf("i before parsing pipe filter : %d\n", i);
	parsePipeFilter(line, pipeFilterArray, commands);
	//there are pipes in the command, enter loop to parse pipes and spaces
	// printf("i: %d\n", i);

	while (strcmp(pipeFilterArray[j],"/0") != 0){
		// printf("\nindex %d pipe filter: %s\n", j, pipeFilterArray[j]);
		parseSpaces(pipeFilterArray[j], parsed, &i);
		j++;
	}
	//i is now pointing to the first empty index is array parsed
	for (int j = 0; j < i; j ++)
	  printf("result arr %d: %s\n", j, parsed[j]);


	// fork processes
	pid_t pid; //keep track of process id of forks
	int fd[2]; //hold fds of both ends of the pipe
	j = 0;//j keeps track of commands array
	i = 0;//i keeps track of parsed array

	//create pipe
	if (pipe(fd) < 0)
		perror("pipe error");

		pid = fork();
		if (pid < 0){
			perror("Problem forking");
			exit(1);
	}
	else if (pid > 0){
		//parent process
		//

		while (strcmp(commands[j], "\0") != 0){ //iterate through command array
			//fork new process for each command
			//execl to perform the command
			//dup output to a pipe  of parent process, and go to next command and create child and repeat
			pid = fork();
			if (pid < 0){
				perror("Problem forking");
				exit(1);
			}
			else if (pid > 0){
				//parent process
				//
				wait(0);
				close(fd[1]) //parent is not going to write, close fd write end
				while (strcmp(parsed[i], "\0") != 0){
					i++;
				}//end of loop, i would now point to keyword of new command
				//return to the while loop
			}
			if (pid == 0){
				//child process
				close(fd[0]); //close the read end, child only needs to write
				if (dup2(fd[1], STDOUT_FILENO) < 0){
					//use dup2 to redirect the writing of fd[1] into shared memory
					//if its less than 0, error has occured
					perror("cant dup");
					exit(1);
				}

				//perform execl function to make the system call
				if (execvp(parsed[i], commands[j]) < 0){//perform the system call and work
					perror("exec problem");
					exit(1);
				}

				//then return the result to the parent process
				//the parents process would then return the result to its parent
				//the loop would continue iterating
			}

		}

	}
	else{
		//child process
	}

	return status;
}
