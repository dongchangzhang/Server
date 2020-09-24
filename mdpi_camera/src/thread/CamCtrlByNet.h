//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_CAMCTRLBYNET_H
#define MDPI_CAMERA_CAMCTRLBYNET_H


#include "ThreadBase.h"

class CamCtrlByNet : public ThreadBase {
public:
    CamCtrlByNet(wxWindow *_handler, int _port);
    void *Entry() override;

private:
    int port;

};


#endif //MDPI_CAMERA_CAMCTRLBYNET_H
