//
// Created by vhdsih on 20-1-20.
//

#include "RS422.h"
#include "RS422.h"
#include "../value/const_values.h"

#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>


RS422::RS422(std::string _dev_name) {
    dev_name = std::move(_dev_name);
    open_dev();
    init_dev();
}

RS422::~RS422() {
    if (fd >= 0) {
        // Waiting for receiver to be terminated
        sleep(1);
        /* restore the old port settings */
        tcsetattr(fd, TCSANOW, &oldtty);
        close(fd);
    }
}

int RS422::open_dev() {

    /*
      Open modem device for reading and writing and not as controlling tty
      because we don't want to get killed if linenoise sends CTRL-C.
    */
    //fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);

    printf("opening device %s\n", dev_name.c_str());

    fd = open(dev_name.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror(dev_name.c_str());
        opened = false;
        return -1;
    }
    opened = true;
    return 0;
}

bool RS422::init_dev() {
    tcgetattr(fd, &oldtty);
    bzero(&tty, sizeof(tty));

    cfsetispeed(&tty, BAUDRATE);
    cfsetospeed(&tty, BAUDRATE);

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= (CLOCAL | CREAD | CS8 | PARENB | CSTOPB);
    tty.c_iflag &= ~(ISTRIP |IXON | IXOFF | IXANY | INLCR | ICRNL | IGNCR);

    tty.c_oflag &= ~(ONLCR | OCRNL);
    tty.c_lflag &= ~ (ICANON | ECHO | ECHOE | ISIG);

    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        opened = false;
        return false;
    }
    return true;
}

void RS422::close_dev() {
    if (fd >= 0) {
        sleep(1);
        tcsetattr(fd, TCSANOW, &oldtty);
        std::cout << "close " << fd << std::endl;
        close(fd);
    }
}

int RS422::send(char *buf, int send_len) {
    int res = write(fd, buf, send_len);
    return res;
}

int RS422::recv(char *buf, int max_len) {
    int res = read(fd, buf, max_len);
    return res;
}

