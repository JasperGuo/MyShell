# include <iostream>
# include <unistd.h>
# include <stdio.h>
# include "parser.cpp"

using namespace std;

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
	void printParserResult(CommandParserResult &result);

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
			printParserResult(commandParserResult);
		}
		//cout<<command <<endl;
	}
	
}

/**
 * show the welcome message
 */
void Mysh::init(){
	cout<<"WelCome to mysh by < 0440031 > " <<endl;
}

/**
 * show current user and current directory
 * show prompt
 */
void Mysh::prompt(){
	cout<<username << " in " << currentDir <<endl;
	cout<<"mysh> ";
}

/**
 * read command
 * @return [description]
 */
string Mysh::readCommand(){
	string command;
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