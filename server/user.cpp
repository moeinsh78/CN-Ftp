#include "user.hpp"

using namespace std;

User::User()
{
}

User::User(std::string username, 
           std::string password, 
           int remaining_download_size,
           std::string is_admin)
           : username(username), 
             password(password), 
             remaining_download_size(remaining_download_size), 
             is_admin(is_admin == "true")
{
}
void User::get_attr()
{
  cout << "Username:\n" << username << endl;
  cout << "Password:\n" << password << endl;
  cout << "remaining_download_size:\n" << remaining_download_size << endl;
  cout << "admin? \n" << is_admin << endl;
}
std::string User::get_username()
{
  return username;
}
std::string User::get_password()
{
  return password;
}