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
    if (cmd == "user")
    {
        login.find_username(users,args[0]);
    }
    else if (cmd == "pass")
    {
        if(login.login(args[0]))
            logged_in = true;
    }
    else if (cmd == "pwd")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "mkd")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "dele")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "ls")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "cwd")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "rename")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "retr")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "help")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else if (cmd == "quit")
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
    else
    {
        if(!logged_in)
            cout << "332: Need account for login." <<endl;
    }
}