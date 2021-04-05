#include "login.hpp"

using namespace std;

void Login::find_username(std::vector<User> users,std::string username){
    logged_in = false;
    user_enter = false;
    for(int i = 0 ; i < users.size() ; i++){
        if(username.compare(users[i].get_username()) == 0){
            user = users[i];
            user_enter = true;
            cout << "331: User name okay,need password."<<endl;
        }
        else
            cout << "430: Invalid username or password"<<endl;
    }
}
bool Login::login(std::string pass){
    if(!user_enter){
        cout << "503: Bad sequenceof commands" <<endl;
        return false;
    }
    if(pass.compare(user.get_password()) == 0){
        logged_in = true;
        cout << "230: User logged in, proceed. Logged out if appropriate."<<endl;
    }
    else
        cout << "430: Invalid username or password"<<endl;
    return logged_in;
}