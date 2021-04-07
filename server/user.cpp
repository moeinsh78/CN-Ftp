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

std::string User::get_username()
{
  return username;
}
std::string User::get_password()
{
  return password;
}
int User::get_remaining_download_size()
{
  return remaining_download_size;
}
void User::reduce_download_size(int dif)
{
  remaining_download_size -= dif;
}
bool User::admin()
{
  return is_admin;
}