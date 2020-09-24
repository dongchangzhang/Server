//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_CAMCTRLBYRS422_H
#define MDPI_CAMERA_CAMCTRLBYRS422_H


#include "ThreadBase.h"
#include "../net/RS422.h"
#include "../value/const_values.h"

class CamCtrlByRs422 : public ThreadBase {
public:
    explicit CamCtrlByRs422(wxWindow *_handler);
    void *Entry() override;

private:
    RS422 device;

};


#endif //MDPI_CAMERA_CAMCTRLBYRS422_H
