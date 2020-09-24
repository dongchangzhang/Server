//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_RS422_H
#define MDPI_CAMERA_RS422_H


#include <termios.h>
#include <string>

class RS422 {
public:
    RS422() = default;

    RS422(std::string dev_name);

    ~RS422();

    int open_dev();
    bool init_dev();
    void close_dev();
    int recv(char *buf, int max_len);
    int send(char *buf, int send_len);
    inline bool is_open() {
        return opened;
    }
    inline std::string get_device_name() {
        return dev_name;
    }

private:
    int fd;
    bool opened = false;
    std::string dev_name;
    struct termios oldtty, tty;
};

#endif //MDPI_CAMERA_RS422_H
