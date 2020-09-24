//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_GNCLOC_H
#define MDPI_CAMERA_GNCLOC_H

#include <wx/wx.h>
#include "../data/GncShower.h"
#include "Image.h"

class GncLoc : public wxFrame {
public:
    GncLoc(wxFrame *_handler);
    void on_close(wxCloseEvent &event);
    void on_timer(wxTimerEvent &event);

private:
    wxFrame *handler;
    wxTimer timer;
    GncShower shower;
    wxImagePanel *img;
    size_t _n_gnc_to_show_ = 0;

private:
DECLARE_EVENT_TABLE()
};


#endif //MDPI_CAMERA_GNCLOC_H
