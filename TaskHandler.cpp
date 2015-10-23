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
	TaskHandler(char** command, int length);

	ExecuteResult handleTask();

	~TaskHandler();
private:
	int matchCommand();
	void showChildPid();
	ExecuteResult buildCommandHandler();
	ExecuteResult changeDirectory();
	
	static const string defined_command[1024];
	static const int defined_command_num = 4;

	char** command_array;
	int command_array_length;
	pid_t pid;

};

const string TaskHandler::defined_command[1024] = {"cd", "fg", "bg", "exit"};

TaskHandler::TaskHandler(){

}

TaskHandler::TaskHandler(char** command, int length){
	command_array_length = length;
	command_array = command;
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
			result = buildCommandHandler();
	}
	return result;
}

/**
 * run build in task
 * @return [description]
 */
ExecuteResult TaskHandler::buildCommandHandler(){

	pid = fork();
	ExecuteResult result;

	result.task_type = defined_command_num;

    	if(pid < 0 ){
        		result.message = "fork fail";
        		result.status = -1;
    	}else if (pid == 0){
    		execvp(command_array[0], command_array);
    	}else{
    		showChildPid();
       		wait(NULL);
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