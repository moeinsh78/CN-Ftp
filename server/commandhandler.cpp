#include "commandhandler.hpp"

using namespace std;

CommandHandler::CommandHandler()
{
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
        remove("temp.txt");
        if (cmd == "user")
        {
            login.find_username(users,args[0]);
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
        
        if (cmd == "pwd")
        {
            system("pwd >> temp.txt");
            ifstream infile("temp.txt");
            string line;
            string str;
            str = "257: ";
            getline(infile, line);
            str += line;
            throw string(str);
        }
        else if (cmd == "mkd")
        {
            
        }
        else if (cmd == "dele")
        {
            
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
                str += " ";
                str += line;
            }
            throw string(str);
            
        }
        else if (cmd == "cwd")
        {
            
        }
        else if (cmd == "rename")
        {
           
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