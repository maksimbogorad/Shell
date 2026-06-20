/* Execution */

#include "../include/common.h"
#include "../include/executor.h"
#include "../include/parser.h"

int redirectIO(const char * inputfile, const char * outputfile, int append_mode)
{
	int fd;
	if (outputfile && !append_mode)
	{
		fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			fprintf(stderr, "Open file error %s\n", outputfile ? outputfile : inputfile);
			return -1;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (inputfile)
	{
		fd = open(inputfile, O_RDONLY, 0666);
		if (fd == -1)
		{
			fprintf(stderr, "Open file error %s\n", outputfile ? outputfile : inputfile);
			return -1;
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (outputfile && append_mode)
	{
		fd = open(outputfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd == -1)
		{
			fprintf(stderr, "Open file error %s\n", outputfile ? outputfile : inputfile);
			return -1;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return 0;
}


int checkSpecialcommands(List * commands, uint16_t listLen)
{
	if (!strcmp(commands->part, "exit"))
		exit(0);
	if (!strcmp(commands->part, "cd"))
	{	
		int reschange = 0;
		if (listLen == 1)
			reschange = chdir(getenv("HOME"));
		else
			reschange = chdir(commands->next->part);
		if (reschange == -1)
			perror(commands->next->part);
		return 1;
	}
	return 0;
}

void executionAlone(Commands * commandsList, int back_mode)
{
	if (commandsList == NULL)
	{
		perror("Empty list error!\n");
		return;
	}

	pid_t proccessPid = fork();

	if (proccessPid < 0)
	{
		perror("Fork error!\n");
		freeCommandsList(commandsList);
		return;
	}
	else if (proccessPid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (redirectIO(commandsList->input_file, commandsList->output_file, commandsList->is_append) == -1)
			exit(1);
		execvp(commandsList->comms[0], commandsList->comms);
		perror("Command not found");
		exit(1);
	}
	if (!back_mode)
		wait(NULL);
	else
	{
		fprintf(stdout, "Proccess pid is %d\n", proccessPid);
		fflush(stdout);
	}
}


void conveyorExecute(Commands * commandsList, int commandsListLength, uint16_t listLen, int back_mode)
{
	pid_t pids[commandsListLength];
	Commands * tmp = commandsList;
	int fd_pipes[commandsListLength-1][2];
	int i = 0;
	pid_t pid;

	for (int j = 0; j < commandsListLength-1; j++)
	{
		if (pipe(fd_pipes[j]) == -1)
		{
			perror("Pipe error\n");
			freeCommandsList(commandsList);
			return;
		}
	}

	while (i < commandsListLength)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Fork error!\n");
			freeCommandsList(commandsList);
			return;
		}
		else if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			if (i > 0)
			{
				dup2(fd_pipes[i-1][0], 0);
			}
			if (i < commandsListLength-1)
			{
				dup2(fd_pipes[i][1], 1);
			}

			for (int j = 0; j < commandsListLength-1; j++)
			{
				close(fd_pipes[j][0]);
				close(fd_pipes[j][1]);
			}

			if (redirectIO(tmp->input_file, tmp->output_file, tmp->is_append) == -1)
				exit(1);
			execvp(tmp->comms[0], tmp->comms);
			perror("Command not found");
			exit(1);
		}
		tmp = tmp->next;
		pids[i] = pid;
		i++;
	}

	for (int j = 0; j < commandsListLength-1; j++)
	{
		close(fd_pipes[j][0]);
		close(fd_pipes[j][1]);
	}

	if (!back_mode)
	{
		for (int j = 0; j < commandsListLength; j++)
		{
			waitpid(pids[j], NULL, 0);
		}
	}
	else
	{
		fprintf(stdout, "Proccess pid is %d\n", pids[0]);
		fflush(stdout);
	}
}

void execute(List * list, uint16_t listLen)
{
	if (checkSpecialcommands(list, listLen))
		return;

	int commandsListLength = 0, back_mode = 0;
	Commands * commandsList = createCommandsList(list, listLen, &commandsListLength, &back_mode);
	if (commandsList == NULL)
	{
		perror("Empty list error!\n");
		return;
	}

	if (commandsListLength == 1)
		executionAlone(commandsList, back_mode);
	else
		conveyorExecute(commandsList, commandsListLength, listLen, back_mode);

	if (!back_mode)
		freeCommandsList(commandsList);
}