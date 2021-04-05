#include "user.hpp"

using namespace std;

User::User(std::string username, 
           std::string password, 
           int remaining_download_size,
           bool is_admin)
           : username(username), 
             password(password), 
             remaining_download_size(remaining_download_size), 
             is_admin(is_admin)
{
}