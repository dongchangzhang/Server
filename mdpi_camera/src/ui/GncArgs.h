//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_GNCARGS_H
#define MDPI_CAMERA_GNCARGS_H

#include <wx/wx.h>

class GncArgs : public wxFrame {
public:
    GncArgs(wxFrame *_handler);
    void on_close(wxCloseEvent &event);
    void on_timer1(wxTimerEvent &event);
    void on_timer2(wxTimerEvent &event);

private:
    size_t n_bits = 0;
    wxFrame *handler;
    wxTimer timer1, timer2;
    wxTextCtrl* raw_gnc, *parsered_gnc;
    unsigned char buffer[512];

private:
DECLARE_EVENT_TABLE()
};


#endif //MDPI_CAMERA_GNCARGS_H
