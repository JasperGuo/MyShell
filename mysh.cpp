#include <iostream>
# include <unistd.h>
#include <stdio.h>

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
