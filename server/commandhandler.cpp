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

void CommandHandler::record_log(string message)
{
    logfile.open("server.log", std::ofstream::out | std::ofstream::app);
    time_t now = time(0);
    logfile << message <<  " -- " << (char*) ctime(&now);
    logfile.close();
}

void CommandHandler::handle(std::vector<User> users, int data_channel_fd)
{
    try
    {
        string log_str;
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
            record_log("User " + login.get_logged_username() + " asked their current working directory");
            throw string(str);
        }
        else if (cmd == "mkd")
        {
            string command;
            command = "mkdir -p " + string(args[0]);
            system(command.c_str());
            string str;
            str = "257: " + args[0] + " created.";
            record_log("User " + login.get_logged_username() + " created " + string(args[0]) + " directory");
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
                    record_log("User " + login.get_logged_username() + " deleted " + string(args[1]) + " file");
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
                    record_log("User " + login.get_logged_username() + " deleted " + string(args[1]) + " directory");
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
            record_log("User " + login.get_logged_username() + " requested a list transfer on their working directory");
            throw string(str);
            
        }
        else if (cmd == "cwd")
        {
            if(args.size() == 0)
            {
                if(!chdir(server_path.c_str()))
                {
                    record_log("User " + login.get_logged_username() + " changed their working directory to " + server_path);
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
                        record_log("User " + login.get_logged_username() + " changed their working directory to " + string(args[0]));
                        throw string("250: Successful change.");
                    }    
            }
            throw string("500: Error");
        }
        else if (cmd == "rename")
        {
            if (rename(args[0].c_str(), args[1].c_str()) != 0)
		        throw string("500: Error");
	        else {
		        record_log("User " + login.get_logged_username() + " changed file name " + string(args[0]) + " to " + string(args[1]));
                throw string("250: Successful change.");
            }
        }
        else if (cmd == "retr")
        {
            ifstream in_file(args[0], ios::binary);
            in_file.seekg(0, ios::end);
            int file_size = in_file.tellg();
            if(file_size > login.get_user().get_remaining_download_size())
                throw string("425: Can't open data connection");
            int file_fd = open(args[0].c_str(),O_RDONLY);
            sendfile(data_channel_fd,file_fd,NULL,file_size);
            record_log("User " + login.get_logged_username() + " downloaded file " + string(args[0]));
            throw string("226: Successful Download.");
        }
        else if (cmd == "help")
        {
            ifstream infile("help.txt");
            string line;
            getline(infile, line);
            string str;
            str = line.c_str();
            while(getline(infile, line))
            {
                str += "\n";
                str += line;
            }
            throw string(str);
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