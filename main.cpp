#include <iostream>
#include <string>

#include "Server.h"
#include "constants.h"


#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>


void gnc() {
    std::string ip = "127.0.0.1";
    int port = GNC_PORT;
    Server server(ip, port);
    server.run();
}
void photo() {
    std::string ip = "127.0.0.1";
    int port = SEND_IMAGE_PORT;
    Server server(ip, port);
    server.run();
}
int main() {
    std::thread th1(gnc);
    std::thread th2(photo);
    th1.join();
    th2.join();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}