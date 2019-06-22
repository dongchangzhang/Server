//
// Created by z on 19-4-26.
//


#ifndef SERVERGUI_APP_H
#define SERVERGUI_APP_H

#include <wx/wx.h>

#include "MainFrame.h"
#include "ImagePanel.h"

class App: public wxApp {
public:
    virtual bool OnInit();

private:
    MainFrame *frame;
private:
};


#endif //SERVERGUI_APP_H
