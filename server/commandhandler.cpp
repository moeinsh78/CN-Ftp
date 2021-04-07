#include "commandhandler.hpp"

using namespace std;


CommandHandler::CommandHandler(string path,vector<std::string> files)
{
    server_path = path;
    client_directory = path;
    system_files = files;
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
bool CommandHandler::in_system_files(string file)
{
    for(int i=0;i<system_files.size();i++)
        if(file == system_files[i])
            return true;
    return false;
}
void CommandHandler::record_log(string message)
{
    string file;
    file = server_path + "/server.log";
    logfile.open(file, std::ofstream::out | std::ofstream::app);
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
                throw string(SYNTAX_ERROR);

        }
        else if (cmd == "pass")
        {
            if(login.login(args[0])){
                logged_in = true;
                record_log("User " + login.get_user().get_username() + " logged in successfully");
                throw string(USER_LOGGEDIN);
            }
            throw string(SYNTAX_ERROR);

        }
        if(!logged_in)
            throw string(LOGIN_ERROR);
        
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
            record_log("User " + login.get_user().get_username() + " asked their current working directory");
            throw string(str);
        }
        else if (cmd == "mkd")
        {
            if(args.size() == 0)
                throw string(SYNTAX_ERROR);
            string command;
            command = "mkdir -p " + string(args[0]);
            system(command.c_str());
            string str;
            str = "257: " + args[0] + " created.";
            record_log("User " + login.get_user().get_username() + " created " + string(args[0]) + " directory");
            throw string(str);
        }
        else if (cmd == "dele")
        {
            if(in_system_files(args[1]) && !login.get_user().admin())
                throw string(FILE_UNAVAILABLE);
            
            if(args.size() < 2)
                throw string(SYNTAX_ERROR);

            if(args[0] == "-f")
            {
                string command;
                command = "rm " + string(args[1]);
                if(!system(command.c_str()))
                {
                    string str;
                    str = "250: " + args[1] + " deleted.";
                    record_log("User " + login.get_user().get_username() + " deleted " + string(args[1]) + " file");
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
                    record_log("User " + login.get_user().get_username() + " deleted " + string(args[1]) + " directory");
                    throw string(str);
                }
            }
            throw string(SYNTAX_ERROR);
            
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
            str += "\n" + string(LIST_TRANS);
            remove("temp.txt");
            record_log("User " + login.get_user().get_username() + " requested a list transfer on their working directory");
            throw string(str);
            
        }
        else if (cmd == "cwd")
        {
            if(args.size() > 1)
                throw string(SYNTAX_ERROR);

            if(args.size() == 0)
            {
                if(!chdir(server_path.c_str()))
                {
                    client_directory = server_path;
                    record_log("User " + login.get_user().get_username() + " changed their working directory to " + server_path);
                    throw string(CHANGE_SUCCESSFUL); 
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
                        record_log("User " + login.get_user().get_username() + " changed their working directory to " + string(args[0]));
                        throw string(CHANGE_SUCCESSFUL);
                    }    
            }
            throw string(ERROR);
        }
        else if (cmd == "rename")
        {
            if(args.size() != 2)
                throw string(SYNTAX_ERROR);

            if(in_system_files(args[0]) && !login.get_user().admin())
                throw string(FILE_UNAVAILABLE);

            if (rename(args[0].c_str(), args[1].c_str()) != 0)
		        throw string(ERROR);
	        else {
		        record_log("User " + login.get_user().get_username() + " changed file name " + string(args[0]) + " to " + string(args[1]));
                throw string(CHANGE_SUCCESSFUL);
            }
        }
        else if (cmd == "retr")
        {
            if(args.size() != 1)
                throw string(SYNTAX_ERROR);

            if(in_system_files(args[0]) && !login.get_user().admin())
                throw string(FILE_UNAVAILABLE);

            ifstream in_file(args[0], ios::binary);
            in_file.seekg(0, ios::end);
            int file_size = in_file.tellg();

            if(file_size > login.get_user().get_remaining_download_size())
                throw string(OPEN_ERROR);

            int file_fd = open(args[0].c_str(),O_RDONLY);
            sendfile(data_channel_fd,file_fd,NULL,file_size);
            login.get_user().reduce_download_size(file_size);

            record_log("User " + login.get_user().get_username() + " downloaded file " + string(args[0]));
            throw string(DOWNLOAD_SUCCESSFUL);
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
                record_log("User " + login.get_user().get_username() + " logged out successfully");
                throw string(QUIT_SUCCESSFUL);
            }
        }
        else
        {
            throw string(SYNTAX_ERROR);
        }
    }
    catch(string excep)
    {
        throw excep;
    }
}