#include "client.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    string config_file_path = argv[1];
    Client ftp_client = Client();
    ftp_client.read_config_file(config_file_path);
    ftp_client.start();
    return 0;
}
