#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 100
#define ARR_SIZE 20

enum OP_TYPE
{
	OP_ERROR = -1,
	OP_OK = 0,
};

int FillArgv(char *buf, char *argv[])
{
	if (NULL == buf || NULL == argv)
	{
		return OP_ERROR;
	}
	int iFirst = 0;
	while(*buf)
	{
		if (0 == iFirst)
		{
			*argv = buf;
			argv++;
			buf++;
			iFirst = 1;
			continue;
		}
		if (' ' == *(buf-1) && '\0' != *buf)
		{
			*argv = buf;
			*(buf-1) = '\0';
			argv++;
		}
		buf++;
	}
	return OP_OK;
}

int main()
{
	char buf[BUF_SIZE];
	char *argv[ARR_SIZE];
	pid_t pid = -1;

	while(1)
	{
		memset(buf, 0, BUF_SIZE);
		memset(argv, 0, sizeof(char *)* ARR_SIZE);
		//gets
		printf("please input $: ");
		gets(buf);
		//fork
		pid = fork();		
		if (pid < 0)
		{
			return -1;
		}
		//child process
		if (0 == pid)
		{
			//fill argv
			if (OP_OK != FillArgv(buf, argv))
			{
				exit(1);
			}
			/*int i = 0;
			for (; i < ARR_SIZE; i++)
			{
				if (NULL == argv[i])
				{
					continue;
				}
				printf("%s\r\n", argv[i]);
			}*/
			//execvp
			if (-1 == execvp(argv[0], argv))
			{
				exit(2);
			}
			exit(0);
		}
		//parent process
		else
		{
			wait(NULL);
		}
	}
}
