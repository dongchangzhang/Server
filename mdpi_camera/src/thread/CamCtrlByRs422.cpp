//
// Created by vhdsih on 20-1-20.
//

#include "CamCtrlByRs422.h"
#include "../value/values.h"
#include <fstream>

CamCtrlByRs422::CamCtrlByRs422(wxWindow *_handler) : ThreadBase(_handler), device(MODEMDEVICE_1) {
}

void *CamCtrlByRs422::Entry() {
#ifdef TEST_WITHOUT_RS422
    std::ifstream is("bytes_ctrl.txt");
#else
    // open device
    if (!device.is_open()) {
        printf("open rs422 device error on %s!\n", device.get_device_name().c_str());
        return nullptr;
    } else {
        printf("open rs422 device yes on %s!\n", device.get_device_name().c_str());
        auto &device_commands_rs422_opened = get_commands_rs422_device_open_state();
        device_commands_rs422_opened = true;
    }

#endif

    int recv_len;
    const int BUF_LEN = 1024;
    unsigned char buffer[BUF_LEN];
    auto &mutex_rs422_device_ctrl = get_mutex_for_camera_params_rs422_device(); // rs422 device locker

    while (!TestDestroy()) {
#ifndef TEST_WITHOUT_RS422
        if (mutex_rs422_device_ctrl->TryLock() == wxMUTEX_BUSY) {
            wxMilliSleep(10);
            continue;
        } // lock rs422 device
#endif

        memset(buffer, 0, BUF_LEN);

#ifdef TEST_WITHOUT_RS422
        int tmp;
        is >> tmp;
        buffer[0] = tmp;
        recv_len = 1;
//        printf("::recv rs422 byte of camera params: %x\n", tmp);
        wxMilliSleep(1000);
#else
        recv_len = device.recv((char*)buffer, BUF_LEN);
        mutex_rs422_device_ctrl->Unlock(); // unlock
#endif

        if (recv_len <= 0) continue;

        while (!write_control_bytes((char*)buffer, size_t(recv_len))) wxMilliSleep(1);
    }
    device.close_dev();
    return nullptr;
}
