# include <iostream>
# include <unistd.h>
# include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
# include "parser.cpp"
# include "TaskHandler.cpp"

using namespace std;

void sigchld_hdl (int sig)
{
	/* Wait for all dead processes.
	 * We use a non-blocking call to be sure this signal handler will not
	 * block if a child was cleaned up in another part of the program. */
	while (waitpid(-1, NULL, WNOHANG) > 0) {

	}
	cout << "finish" <<endl;
}

class Mysh
{
public:
	Mysh();
	void run();
	~Mysh();
private:
	void init();
	void prompt();
	string readCommand();
	string username;
	string currentDir;
	void mysh_exit();
	void handleExecuteResult(ExecuteResult &result);
	ExecuteResult handleParseResult();
	void printParserResult(CommandParserResult &result);
	void printExecuteResult(ExecuteResult &result);
	static void sigchld_hdl (int sig);
	Parser parser;
	CommandParserResult commandParserResult;
};

int main(int argc, char const *argv[])
{
	Mysh mysh = Mysh();
	mysh.run();
	return 0;
}

Mysh::Mysh(){
	username = getlogin();
	currentDir = get_current_dir_name();
	parser = Parser();
}

Mysh::~Mysh(){

}

void Mysh::sigchld_hdl (int sig)
{
	/* Wait for all dead processes.
	 * We use a non-blocking call to be sure this signal handler will not
	 * block if a child was cleaned up in another part of the program. */
	while (waitpid(-1, NULL, WNOHANG) > 0) {
		
	}
 	struct  sigaction act;
 	act.sa_handler = Mysh::sigchld_hdl;
 	sigaction(SIGCHLD, &act, 0);
 	// cout << "finish" <<endl;
}

/**
 * control the loop of the shell
 */
void Mysh::run(){
	string command;	
	init();
	while(1){		
		prompt();	
		command = readCommand();
		if (command.length())
		{
			/* code */
			commandParserResult = parser.parseCommand(command);

			//printParserResult(commandParserResult);

			ExecuteResult result = handleParseResult();
			handleExecuteResult(result);
		}
		//cout<<command <<endl;
	}
	
}

/**
 * show the welcome message
 */
void Mysh::init(){

 	signal(SIGINT, SIG_IGN);
 	signal(SIGTSTP, SIG_IGN);

 	struct  sigaction act;
 	act.sa_handler = Mysh::sigchld_hdl;
 	sigaction(SIGCHLD, &act, 0);
 	
	cout<<"WelCome to mysh by < 0440031 > " <<endl;

}

/**
 * show current user and current directory
 * show prompt
 */
void Mysh::prompt(){
	cout<< endl<<"\033[36m" << username  << "\033[0m"   << " in " << "\033[33m" <<  currentDir  <<  "\033[0m" <<endl;
	cout<< "\033[30mmysh> \033[0m" ;
}

/**
 * read command
 * @return [description]
 */
string Mysh::readCommand(){
	string command = "";
	cin.clear();
	getline(cin, command);
	return command;
}

/**
 * show the parse result
 * @param result [description]
 */
void Mysh::printParserResult(CommandParserResult &result){

	cout << "------------------------------------------------------------" << endl;
	cout << "COMMAND TYPE: " << result.command_type << endl;
	cout << "COMMAND ARRAY LENGTH " << result.command_array_length << endl;
	for (int i = 0; i < result.command_array_length; ++i)
	{
		/* code */
		cout << result.command_array[i] << endl;
	}
	cout << "------------------------------------------------------------" << endl;

}

/**
 * show the execute result
 * @param result [description]
 */
void Mysh::printExecuteResult(ExecuteResult &result){
	cout << "------------------------------------------------------------" << endl;
	cout << "TASK TYPE: " << result.task_type << endl;
	cout << "TASK STATUS: " << result.status << endl;
	cout << "TASK MESSAGE: " << result.message << endl;
	cout << "------------------------------------------------------------" << endl;
}

/**
 * according to the parse result (command_type) invoke different module
 * to run the command
 * @return [description]
 */
ExecuteResult Mysh::handleParseResult(){
	ExecuteResult executeResult;
	TaskHandler handler;
	switch(commandParserResult.command_type){
		case 0:
			handler = TaskHandler(
				commandParserResult.command_array,
				commandParserResult.command_array_length,
				0,
				0);
			executeResult = handler.handleTask();
			// printExecuteResult(executeResult);
			break;
		case 1:
			handler = TaskHandler(
				commandParserResult.command_array,
				commandParserResult.command_array_length,
				1,
				0);
			executeResult = handler.handleTask();
			// printExecuteResult(executeResult);
			break;	
		case 2:
			handler = TaskHandler(
				commandParserResult.command_array,
				commandParserResult.command_array_length,
				0,
				1
				);		
			executeResult = handler.handleTask();
			// printExecuteResult(executeResult);
			break;	
		case 3:
			handler = TaskHandler(
				commandParserResult.command_array,
				commandParserResult.command_array_length,
				1,
				1
				);		
			executeResult = handler.handleTask();
			// printExecuteResult(executeResult);
			break;	
	}
	return executeResult;
}

/**
 * according to the execution result, mysh makes some change
 * @param result [description]
 */
void Mysh::handleExecuteResult(ExecuteResult &result){
	if (result.status >= 0)
	{
		/* code */
		switch(result.task_type){
			case 0:
				currentDir = get_current_dir_name();
				break;
			case 3:
				mysh_exit();
				break;
		}
	}else{	
		cout<< result.message << endl;
	}
}

void Mysh::mysh_exit(){
	cout << "goodbye ! " << endl;
	exit(0);
}