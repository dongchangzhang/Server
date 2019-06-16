//
// Created by z on 19-4-26.
//


#ifndef SERVERGUI_MYAPP_H
#define SERVERGUI_MYAPP_H

#include <wx/wx.h>

#include "MyFrame.h"
#include "MyImgPanel.h"

class MyApp: public wxApp {
public:
    virtual bool OnInit();

private:
    MyFrame *frame;
private:
};


#endif //SERVERGUI_MYAPP_H
