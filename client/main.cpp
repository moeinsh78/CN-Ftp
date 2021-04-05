#include "client.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    Client ftp_client = Client();
    ftp_client.start();
    return 0;
}
