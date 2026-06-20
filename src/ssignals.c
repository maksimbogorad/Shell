/* Working with signals */

#include "../include/common.h"
#include "../include/ssignals.h"

void initSignals()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, handlerChild);
}

void checkZombie()
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handlerChild(int sig)
{
	checkZombie();
}
