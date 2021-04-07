#include "login.hpp"

using namespace std;

Login::Login()
{
}

void Login::record_log(string message)
{
    logfile.open("server.log", std::ofstream::out | std::ofstream::app);
    time_t now = time(0);
    logfile << message <<  " -- " << (char*) ctime(&now);
    logfile.close();
}

void Login::find_username(std::vector<User> users,std::string username){
    if(logged_in)
        throw string("500: Error");
    for(int i = 0 ; i < users.size() ; i++){
        if(username.compare(users[i].get_username()) == 0){
            user = users[i];
            found_flag = true;
            break;
        }
    }
    if(found_flag)
        throw string("331: User name okay,need password.");
    
    else 
        throw string("430: Invalid username or password");
}
bool Login::login(std::string pass){
    if(logged_in){
        throw string("500: Error");
    }
    if(!found_flag){
        throw string("503: Bad sequence of commands");
    }
    if(pass.compare(user.get_password()) == 0){
        logged_in = true;
        record_log("User " + user.get_username() + " logged in successfully");
        return true;
    }
    else
        throw string("430: Invalid username or password");
}
bool Login::quit()
{
    if(!logged_in)
        throw string("500: Error");
    logged_in = false;
    found_flag = false;
    record_log("User " + user.get_username() + " logged out successfully");
    return true;
}

string Login::get_logged_username()
{
    return user.get_username();
}