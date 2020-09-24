//
// Created by vhdsih on 20-1-20.
//

#include "GncRecvByRs422.h"
#include "../value/values.h"
#include <fstream>

GncRecvByRs422::GncRecvByRs422(wxWindow *_handler) : ThreadBase(_handler), device(MODEMDEVICE_2) {
}

void *GncRecvByRs422::Entry() {
#ifdef TEST_WITHOUT_RS422
    std::ifstream is("bytes_gnc.txt");
#else
    if (!device.is_open()) {
        printf("open rs422 device error on %s\n", device.get_device_name().c_str());
        return nullptr;
    } else {
        auto &device_gnc_rs422_opened = get_gnc_rs422_device_open_state();
        device_gnc_rs422_opened = true;
        printf("open rs422 device yes on %s\n", device.get_device_name().c_str());
    }
#endif

    int recv_len;
    const int BUF_LEN = 1024;
    unsigned char buffer[BUF_LEN];

    while (!TestDestroy()) {
        memset(buffer, 0, BUF_LEN);

#ifdef TEST_WITHOUT_RS422
        int tmp;
        is >> tmp;
        buffer[0] = tmp;
        recv_len = 1;
//        printf("::recv rs422 byte of gnc: %x\n", tmp);
        wxMilliSleep(100);
#else
        recv_len = device.recv((char*)buffer, BUF_LEN);
#endif
        if (recv_len <= 0) continue;
        while (!write_gnc_bytes((char*)buffer, size_t(recv_len))) wxMilliSleep(1);
        while (!write_show_gnc_bytes((char*)buffer, size_t(recv_len))) wxMilliSleep(1);
    }
    device.close_dev();
    return nullptr;
}
