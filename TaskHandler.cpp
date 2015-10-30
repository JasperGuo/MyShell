# include <iostream>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
#include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

using namespace std;

/**
 * define the return type
 * status --------- -1 fail || 0 success
 */
struct ExecuteResult
{
	int status;
	int task_type;
	string message;
};



class TaskHandler
{
public:
	TaskHandler();
	TaskHandler(char** command, int length, int is_bg, int is_pipe);

	static void setPid(int id);
	static pid_t getPid();

	ExecuteResult handleTask();

	~TaskHandler();
private:
	int matchCommand();

	void showChildPid();
	void showBgChildPid();
	void showChildPid(int id);

	ExecuteResult buildCommandHandler();
	ExecuteResult changeDirectory();

	char** getPipeChildProcessCommand(int sequence);
	int getPipeChildProcessNum();
	ExecuteResult handlePipeline();
	void forkPipeChild(int numOfChildProcess);
	
	static const string defined_command[1024];
	static const int defined_command_num = 4;

	char** command_array;
	int command_array_length;
	static pid_t pid;
	int is_background;
	int is_pipeline;
};

pid_t TaskHandler::pid = 0;

const string TaskHandler::defined_command[1024] = {"cd", "fg", "bg", "exit"};

/**
 * set the child process pid
 * @param id [description]
 */
void TaskHandler::setPid(int id){
	TaskHandler::pid = id;
}

pid_t TaskHandler::getPid(){
	return TaskHandler::pid;
}

TaskHandler::TaskHandler(){

}

TaskHandler::TaskHandler(char** command, int length, int is_bg, int is_pi){
	command_array_length = length;
	command_array = command;
	is_background = is_bg;
	is_pipeline = is_pi;
}

TaskHandler::~TaskHandler(){

}

/**
 * judge whether it's defined command 
 * @return [description]
 */
int TaskHandler::matchCommand(){
	for (int i = 0; i < defined_command_num; ++i)
	{
		/* code */
		if (command_array[0] == defined_command[i])
		{
			/* code */
			return i;
		}
	}
	return defined_command_num;
}

ExecuteResult TaskHandler::handleTask(){
	int type = matchCommand();
	ExecuteResult result;
	switch(type){
		case 0 :
			// do some thing to change the directory
			// cout << " do some thing to change the directory "<<endl;
			result = changeDirectory();
			break;
		case 1:
			// do some thing to push the background task to forgeground
			cout << " do some thing to push the background task to forgeground " <<endl;
			break;
		case 2:
			//  do something to push the forgeground task to background
			cout << "do something to push the forgeground task to background" << endl;
			break;
		case 3: 
			// do something to exit
			cout <<  "do something to exit "<< endl;
			result.task_type = 3;
			result.message = "exit";
			result.status = 0;
			break;	
		default:
			if (is_pipeline)
			{
				/* code */
				result = handlePipeline();
				//getPipeChildProcessCommand(0);
				//getPipeChildProcessCommand(1);
			}else{
				result = buildCommandHandler(); 
			}
	}
	return result;
}

/**
 * run build in task
 * @return [description]
 */
ExecuteResult TaskHandler::buildCommandHandler(){

	pid_t pid = fork();
	setPid(pid);
	ExecuteResult result;

	result.task_type = defined_command_num;
    	if(pid < 0 ){
        		result.message = "fork fail";
        		result.status = -1;
    	}else if (pid == 0){
    		int status = execvp(command_array[0], command_array);
    		if (status != 0)
		{
			/* code */
			cout <<strerror(errno) <<endl;
			exit(status);
		}
    	}else{
    		if (is_background)
    		{
    			showBgChildPid();
    			// wait(NULL);
    		}else{
    			// run in foreground
    			showChildPid();
			int status;
        			waitpid(pid, &status, 0);
        			pid = 0;
    		}
       		result.message = "success";
       		result.status = 0;
    	}
    	return result;
}

/**
 * show the pid
 */
void TaskHandler::showChildPid(){
	cout << "Command executed by pid = " << pid << endl;
}

/**
 * show the background job pid
 */
void TaskHandler::showBgChildPid(){
	cout << "Command executed by pid = " << pid << " in background" << endl;
}

void TaskHandler::showChildPid(int id){
	cout << "Command executed by pid = " << id << endl;
}

/**
 * change the current directory
 * @return [description]
 */
ExecuteResult TaskHandler::changeDirectory(){
	ExecuteResult result;
	result.task_type = 0;
	if (command_array_length < 2)
	{
		/* code */
		result.status = -1;
		result.message = "without path";
	}else{
		result.status = chdir(command_array[1]);	
		if (result.status != 0)
		{
			/* code */
			result.message = strerror(errno);
		}else{
			result.message = "success";
		}
	}
	return result;
}

/**
 * handle pipelint recursively fork process
 * @return [description]
 */
ExecuteResult TaskHandler::handlePipeline(){
	pid = fork();
	ExecuteResult result;
	int total = getPipeChildProcessNum();
	result.task_type = defined_command_num;
    	if(pid < 0 ){
        		result.message = "fork fail";
        		result.status = -1;
    	}else if (pid == 0){
		forkPipeChild(total);
    	}else{
		showChildPid();
		if(!is_background){
			int status;
        			waitpid(pid, &status, 0);
        			pid = 0;
		}
       		result.message = "success";
       		result.status = 0;
    	}
    	return result;
}

/**
 * return the the sequence th command
 * @param  sequence [description]
 * @return          [description]
 */
char** TaskHandler::getPipeChildProcessCommand(int sequence){
	int vertical_num = 0;
	char ** command;
	for (int i = 0; i < command_array_length; ++i)
	{
		if (vertical_num == sequence)
		{
			/* code */
			int j = i;
			for (; j < command_array_length; ++j)
			{
				/* code */
				if (*(command_array[j]) == '|')
				{
					/* code */
					vertical_num++;
					break;
				}
			}
			int length = j - i;
			command = new char*[length];
			for (int k = i; k <= j-1; ++k)
			{
				command[k - i] = command_array[k];
			}
			break;
		}
		if (*(command_array[i]) == '|')
		{
			/* code */
			vertical_num++;
		}
	}
	return command;
}

/**
 * get the number of pipe child processes
 * @return [description]
 */
int TaskHandler::getPipeChildProcessNum(){
	int num = 0;
	for (int i = 0; i < command_array_length; ++i)
	{
		/* code */
		if (*(command_array[i]) == '|')
		{
			/* code */
			num++;
		}
	}
	return num+1;
}

void TaskHandler::forkPipeChild(int numOfChildProcess){
	int total = getPipeChildProcessNum();
	if (numOfChildProcess > 1)
	{
		/* code */
		pid_t childPid;
		int fd[2];
		pipe(fd);
		childPid = fork();
		if (childPid < 0)
		{
			/* code */
			cout << "fork fail" <<endl;
			return;
		}else if (childPid == 0)
		{
			/* code */
			// close stdin, duplicate the input side of pipe to stdin
       			dup2(fd[0], 0);   /* replace stdin */
        			close(fd[0]);
        			close(fd[1]);
        			forkPipeChild(numOfChildProcess - 1);
		}else{
			// close stdout , duplicate the output side of the pipe of stdout
			showChildPid(childPid);
    			char ** command = getPipeChildProcessCommand(total - numOfChildProcess);
        			dup2(fd[1], 1);   /* replace stdout */
        			close(fd[0]);
        			close(fd[1]);
			int status = execvp(command[0], command);
	    		if (status != 0)
			{
				/* code */
				cout <<strerror(errno) <<endl;
				exit(status);
			}
			int childe_proc_status;
        			waitpid(childPid, &childe_proc_status, 0);
		}
	}else{
		char ** command = getPipeChildProcessCommand(total - numOfChildProcess);
		int status = execvp(command[0], command);
    		if (status != 0)
		{
			/* code */
			cout <<strerror(errno) <<endl;
			exit(status);
		}
	}
   }