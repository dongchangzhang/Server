//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_GNCRECVBYRS422_H
#define MDPI_CAMERA_GNCRECVBYRS422_H


#include "ThreadBase.h"
#include "../net/RS422.h"

class GncRecvByRs422 : public ThreadBase {
public:
    explicit GncRecvByRs422(wxWindow *_handler);
    void *Entry() override;

private:
    RS422 device;

};


#endif //MDPI_CAMERA_GNCRECVBYRS422_H
