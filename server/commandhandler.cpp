#include "commandhandler.hpp"

using namespace std;


CommandHandler::CommandHandler(string path)
{
    server_path = path;
    client_directory = path;
}

void CommandHandler::command_parser(string str)
{
    args.clear();
    istringstream ss(str);
    string word;
    ss >> word;
    cmd = word;
    while (ss >> word) 
    {
        args.push_back(word);
    }
}
void CommandHandler::handle(std::vector<User> users)
{
    try
    {
        if (cmd == "user")
        {
            if(args.size()>0)
                login.find_username(users,args[0]);
            else
                throw string("501: Syntax error in parameters or arguments");

        }
        else if (cmd == "pass")
        {
            if(login.login(args[0])){
                logged_in = true;
                throw string("230: User logged in, proceed. Logged out if appropriate.");
            }
        }
        if(!logged_in)
            throw string("332: Need account for login.");
        
        chdir(client_directory.c_str());
        if (cmd == "pwd")
        {
            system("pwd >> temp.txt");
            ifstream infile("temp.txt");
            string line;
            string str;
            str = "257: ";
            getline(infile, line);
            str += line;
            remove("temp.txt");
            throw string(str);
        }
        else if (cmd == "mkd")
        {
            string command;
            command = "mkdir -p " + string(args[0]);
            system(command.c_str());
            string str;
            str = "257: " + args[0] + " created.";
            throw string(str);
        }
        else if (cmd == "dele")
        {
            if(args[0] == "-f")
            {
                string command;
                command = "rm " + string(args[1]);
                if(!system(command.c_str()))
                {
                    string str;
                    str = "250: " + args[1] + " deleted.";
                    throw string(str);
                }
            }
            if(args[0] == "-d")
            {
                string command;
                command = "rmdir " + string(args[1]);
                if(!system(command.c_str()))
                {
                    string str;
                    str = "250: " + args[1] + " deleted.";
                    throw string(str);
                }
            }
            throw string("500: Error");
            
        }
        else if (cmd == "ls")
        {
            system("ls >> temp.txt");
            ifstream infile("temp.txt");
            string line;
            getline(infile, line);
            string str;
            str = line.c_str();
            while(getline(infile, line))
            {
                if(line == "temp.txt")
                    continue;
                str += "\n";
                str += line;
            }
            str += "\n226: List transfer done.";
            remove("temp.txt");
            throw string(str);
            
        }
        else if (cmd == "cwd")
        {
            if(args.size() == 0)
            {
               if(!chdir(server_path.c_str()))
               {
                   throw string("250: Successful change."); 
               }
            }
            else
            {
                if(!chdir(client_directory.c_str()))
                    if(!chdir(args[0].c_str())){
                        system("pwd >> temp.txt");
                        ifstream infile("temp.txt");
                        getline(infile, client_directory);
                        remove("temp.txt");
                        throw string("250: Successful change.");
                    }    
            }
            throw string("500: Error");
        }
        else if (cmd == "rename")
        {
            if (rename(args[0].c_str(), args[1].c_str()) != 0)
		        throw string("500: Error");
	        else
		        throw string("250: Successful change.");
        }
        else if (cmd == "retr")
        {
            
        }
        else if (cmd == "help")
        {
            
        }
        else if (cmd == "quit")
        {
            if(login.quit())
            {
                logged_in = false;
                throw string("221: Successful Quit.");
            }
        }
        else
        {
            
        }
    }
    catch(string excep)
    {
        throw excep;
    }
    
    
}