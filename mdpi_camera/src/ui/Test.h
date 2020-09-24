//
// Created by vhdsih on 20-1-20.
//

#ifndef MDPI_CAMERA_TEST_H
#define MDPI_CAMERA_TEST_H


#include <wx/wx.h>
#include "Image.h"
#include "../data/DataInjection.h"

class Test : public wxFrame{
public:
    Test(wxFrame *handler);
    void on_close(wxCloseEvent &event);
    void on_btn_camera_state(wxCommandEvent &event);
    void on_btn_image_mode(wxCommandEvent &event);
    void on_btn_win_args(wxCommandEvent &event);
    void on_btn_send_image(wxCommandEvent &event);
//    void on_update_image(wxThreadEvent &event);

    inline void set_recv_image(wxImage &_image) {
        image = _image;
    }

private:
    wxImage image;
    wxWindow *handler;
    wxButton *btn_win, *btn_recv;
    wxTextCtrl *text_i, *text_j, *text_ni, *text_nj;
    wxRadioBox *radio_camera_state, *radio_image_mode;
    wxImagePanel *recved_image;

    DataInjection dij;

private:
DECLARE_EVENT_TABLE()
};

#endif //MDPI_CAMERA_TEST_H
