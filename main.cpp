#include <iostream>
#include <string>

#include "net/Server.h"
#include "constants.h"
#include "net/Client.h"


#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>


void gnc(std::string ip, int port) {
    bool stop = false;
    Client client(ip, port);
    std::chrono::milliseconds dura(2500);
    std::chrono::milliseconds dura_long(15000);
    while (!stop) {
        while (client.send_gnc() == -1) {
            std::this_thread::sleep_for(dura);
        }
        std::this_thread::sleep_for(dura_long);
    }
}

void photo(int port) {
    bool stop = false;
    Server server(port);
    std::chrono::milliseconds dura(250);
    while (!stop) {
        server.recv_photo();
        std::this_thread::sleep_for(dura);
    }

}
int main() {
    std::thread th1(gnc, IP, GNC_PORT);
    std::thread th2(photo, SEND_IMAGE_PORT);
    th1.join();
    th2.join();
    return 0;
}