//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_GNCRECVBYNET_H
#define MDPI_CAMERA_GNCRECVBYNET_H


#include "ThreadBase.h"
#include <string>

class GncRecvByNet : public ThreadBase {
public:
    GncRecvByNet(wxWindow *_handler, int _port);
    void *Entry() override;
private:
    int port;
    unsigned long long ngnc = 0;
};


#endif //MDPI_CAMERA_GNCRECVBYNET_H
