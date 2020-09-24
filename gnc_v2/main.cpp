#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TEST_ON_LOCALHOST 1

class Client {
public:
    Client(std::string _ip, int _port) : ip(_ip), port(_port) {
        auto y = init();
        std::cout << "sockets start? " << y << std::endl;
    }

    ~Client() {
        close(fd);
    }

    int send(char buf[], int send_len);
    void close_client();

private:
    bool init();

private:
    int fd, port;
    socklen_t len;
    std::string ip;
    sockaddr_in server;
};


bool Client::init() {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cerr << "create socket error!" << std::endl;
        return false;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
#ifdef TEST_ON_LOCALHOST
    server.sin_addr.s_addr = htonl(INADDR_ANY);
#else
    server.sin_addr.s_addr = inet_addr(ip.c_str());
#endif
    len = sizeof(server);
    return true;
}


int Client::send(char *buf, int send_len) {
    return sendto(fd, buf, send_len, 0, (struct sockaddr*) &server, sizeof(server));
}

void Client::close_client() {
    close(fd);
}

time_t time_2_seconds(int year, int month, int day, int hour, int minute, int second) {
    tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = (year - 1900);
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    printf("%ld\n", timegm(&t));
    return timegm(&t);
}

void seconds_2_time(time_t timestamp, tm *t) {
    auto *p = localtime(&timestamp);
    memcpy(t, p, sizeof(tm));
}

void int_2_bytes(int x, char buffer[]) {
    for (int i = 0; i < 4; ++i) {
        buffer[4 - i - 1] = ((unsigned char*)&x)[i];
    }
}

void double_2_bytes(double x, char buffer[]) {
    for (int i = 0; i < 8; ++i) {
        buffer[8 - i - 1] = ((unsigned char*)&x)[i];
    }
}
void bytes_2_double(char buffer[], double &x) {
    for (int i = 0; i < 8; ++i) {
        ((unsigned char*)&x)[i] = buffer[8 - i - 1];
    }
}
int main() {
    char buffer[512];
    int int_tmp = 0;
    double double_tmp = 0;
    Client client("192.168.0.5", 5003);

    time_t timestamps = time_2_seconds(2018, 1, 1, 0, 0, 0);
    tm t;

    double len = 5000000, angle = 0;

    for (int i = 0; i < 10000; ++i) {
        timestamps += 600;
        seconds_2_time(timestamps, &t);

        printf("%d %d %d\n", t.tm_hour, t.tm_min, t.tm_sec);
        memset(buffer, 0, 512);
        int addr = 0;
        int addr_x = 0, addr_y = 0, addr_z = 0;
        // 4 year
        int_tmp = t.tm_year + 1900;

        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 4 month
        int_tmp = t.tm_mon + 1;
        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 4 day
        int_tmp = t.tm_mday;
        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 4 hour
        int_tmp = t.tm_hour;
        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 4 minute
        int_tmp = t.tm_min;
        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 4 second
        int_tmp = t.tm_sec;
        int_2_bytes(int_tmp, &buffer[addr]);
        addr += 4;
        // 32 q
        addr += 32;
        // 8 pitch
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 yaw
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 roll
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 pitch speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 yaw speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 roll speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 distance
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 sun x
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr_x = addr;
        addr += 8;
        // 8 sun y
        double_tmp = 0;
        addr_y = addr;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 sun z
        double_tmp = 1;
        addr_z = addr;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 1
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 2
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 3
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 4
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 5
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 orbit 6
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 location x
        double_tmp = 10000 + len * cos(angle);
        double_2_bytes(double_tmp, &buffer[addr]);
        double_2_bytes(double_tmp, &buffer[addr_x]);
        addr += 8;
        // 8 location y
        double_tmp =  10000 + len * sin(angle);
        double_2_bytes(double_tmp, &buffer[addr]);
        double_2_bytes(double_tmp, &buffer[addr_y]);
        addr += 8;
        // 8 location z
        double_tmp = -446294;
        double_2_bytes(double_tmp, &buffer[addr]);
        double_2_bytes(double_tmp, &buffer[addr_z]);
        addr += 8;
        // 8 location x speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 location y speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        // 8 location z speed
        double_tmp = 0;
        double_2_bytes(double_tmp, &buffer[addr]);
        addr += 8;
        printf("sleep %d\n", i);
        client.send(buffer, addr);
        angle += 6.28 / 100;
        if (i % 100 == 0) len += 2000000;
        sleep(1);
    }
    return 0;
}
