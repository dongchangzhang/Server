//
// Created by vhdsih on 2020/1/23.
//

#ifndef MDPI_CAMERA_GNCPARSER_H
#define MDPI_CAMERA_GNCPARSER_H


#include "ThreadBase.h"

class GncParser : public ThreadBase {
public:
    explicit GncParser(wxWindow *_handler);
    void *Entry() override;
};


#endif //MDPI_CAMERA_GNCPARSER_H
