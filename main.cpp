#include <iostream>
#include <string>

#include "Server.h"
#include "constants.h"


#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>


void gnc(std::string ip, int port) {
    Server server(ip, port);
    server.run();
}
void photo(std::string ip, int port) {
    Server server(ip, port);
    server.run();
}
int main() {
    std::thread th1(gnc,CLIENT_IP_THINKPAD, GNC_PORT);
    std::thread th2(photo, CLIENT_IP_THINKPAD, SEND_IMAGE_PORT);
    th1.join();
    th2.join();
    return 0;
}