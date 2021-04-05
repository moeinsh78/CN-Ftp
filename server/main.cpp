#include "server.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    string config_file_path = argv[1];
    Server ftp_server = Server();
    ftp_server.read_config_file(config_file_path);
    ftp_server.start();
    return 0;
}
