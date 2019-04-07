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
    std::chrono::milliseconds dura_long(5000);
    while (!stop) {
        while (client.send_gnc() == -1) {
            std::this_thread::sleep_for(dura);
        }
        std::this_thread::sleep_for(dura);
    }
}
//void gnc(std::string ip, int port) {
//    bool stop = false;
//    Server client(ip, port);
//    std::chrono::milliseconds dura(2500);
//    std::chrono::milliseconds dura_long(5000);
//    client.recv_hello();
//    while (!stop) {
//        client.send_gnc();
//        std::this_thread::sleep_for(dura);
//    }
//}
void photo(std::string ip, int port) {
    bool stop = false;
    Server server(ip, port);
    std::chrono::milliseconds dura(250);
    while (!stop) {
        server.recv_photo();
        std::this_thread::sleep_for(dura);
    }

}
int main() {
    std::thread th1(gnc,CLIENT_IP_THINKPAD, GNC_PORT);
    std::thread th2(photo, CLIENT_IP_THINKPAD, SEND_IMAGE_PORT);
    th1.join();
    th2.join();
    return 0;
}