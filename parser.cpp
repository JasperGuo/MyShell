# include <iostream>

using namespace std;

/**
 * command_type----0:  without & and |
 * command_type----1:  only &
 * command_type----2:  only |
 * command_type----3:  both | and &
 */
struct CommandParserResult
{
	int command_type;
	int command_array_length;
	char** command_array;
};

class Parser
{
public:
	Parser();
	CommandParserResult parseCommand(string command);
	~Parser();
private:
	int getCommandType(bool has_vertical, bool has_and);
};

Parser::Parser(){

}

Parser::~Parser(){

}

/**
 * get the command type
 * @param command_array        [description]
 * @param command_array_length [description]
 * @param has_vertical         [description]
 * @param has_and              [description]
 */
int Parser::getCommandType(bool has_vertical, bool has_and){
	int command_type;
	if (has_vertical)
	{
		/* code */
		if (has_and)
		{
			/* code */
			command_type = 3;

		}else{
			command_type = 2;
		}
	}else{
		if (has_and)
		{
			/* code */
			command_type = 1;
		}else{
			command_type = 0;
		}
	}

	return command_type;
}


/**
 * parse the command
 * @param command [description]
 */
CommandParserResult Parser::parseCommand(string command){
	string command_array[1024];
    	int command_array_length = 0;

    	bool has_vertical = false;
    	bool has_and = false;

    	for(int i = 0; i<command.length(); i++){
        		if(command[i] == '|'){
        			has_vertical = true;
            			command_array[command_array_length] = command[i];
            			command_array_length++;
        		}else if(command[i] == '&'){
        			has_and = true;
            			// command_array[command_array_length] = command[i];
            			// command_array_length++;
        		}else if(command[i] != ' ' ){
            			int j = i + 1;
            			for(; j < command.length(); j++){
                			if(command[j] == ' ' || command[j] == '|' || command[j] == '&'){
                   				 break;
                			}
            			}
            			command_array[command_array_length] = command.substr(i, j-i);
            			command_array_length++;
            			i = j-1;
        		}
    	}


    	int command_type =  getCommandType(has_vertical, has_and);
	CommandParserResult commandParserResult;
	commandParserResult.command_type = command_type;
	commandParserResult.command_array_length = command_array_length;

	commandParserResult.command_array = new char*[command_array_length+1];
	for (int i = 0; i < command_array_length; ++i)
	{
		/* code */
		commandParserResult.command_array[i] = &command_array[i][0u];
	}
	commandParserResult.command_array[command_array_length] = NULL;

	return commandParserResult;
}

