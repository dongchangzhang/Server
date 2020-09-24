//
// Created by vhdsih on 2020/1/23.
//

#ifndef MDPI_CAMERA_CAMCTRLPARSER_H
#define MDPI_CAMERA_CAMCTRLPARSER_H


#include "ThreadBase.h"
#include "../net/RS422.h"

class CamCtrlParser : public ThreadBase {
public:
    explicit CamCtrlParser(wxWindow *_handler);
    void *Entry() override;

private:
    RS422 device;

};


#endif //MDPI_CAMERA_CAMCTRLPARSER_H
