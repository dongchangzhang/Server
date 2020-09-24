//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_CAMARGS_H
#define MDPI_CAMERA_CAMARGS_H


#include <wx/wx.h>
class CamArgs : public wxFrame {

public:
    wxFrame *handler;
    CamArgs(wxFrame *handler);
    void on_close(wxCloseEvent &event);
    wxStaticText *work_mode, *image_mode, *win_args;
private:
DECLARE_EVENT_TABLE()
};


#endif //MDPI_CAMERA_CAMARGS_H
