# include <iostream>
# include <sys/types.h>
# include <sys/wait.h>
#include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

using namespace std;

class TaskHandler
{
public:
	TaskHandler();
	TaskHandler(string command[], int length);

	int handleTask();

	~TaskHandler();
private:
	void showChildPid();

	int buildCommandHandler();

	char** command_array;
	int command_array_length;
	pid_t pid;

};

TaskHandler::TaskHandler(){

}

int main(int argc, char const *argv[])
{
	string command[1024] = {"sleep", "3"};
	TaskHandler taskHandler = TaskHandler(command, 2);
	taskHandler.handleTask();
	return 0;
}

TaskHandler::TaskHandler(string command[], int length){
	command_array_length = length;

	command_array = new char*[length+1];
	for (int i = 0; i < length; ++i)
	{
		/* code */
		command_array[i] = &command[i][0u];
	}
	command_array[length] = NULL;
}

TaskHandler::~TaskHandler(){

}

int TaskHandler::handleTask(){
	return buildCommandHandler();
}

/**
 * run build in task
 * @return [description]
 */
int TaskHandler::buildCommandHandler(){

	pid = fork();
    	if(pid < 0 ){
        		cout<< " Fork, Failed" << endl;
		return 1;
    	}else if (pid == 0){
    		execvp(command_array[0], command_array);
    	}else{
    		showChildPid();
       		wait(NULL);
        		cout<<"Child Complete"<<endl;
    	}
    	return 0;
}

/**
 * show the pid
 */
void TaskHandler::showChildPid(){
	cout << "Command executed by pid = " << pid << endl;
}
