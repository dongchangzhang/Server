//
// Created by vhdsih on 20-1-18.
//

#ifndef MDPI_CAMERA_HOME_H
#define MDPI_CAMERA_HOME_H

#include <wx/wx.h>


#include "Image.h"
#include "Test.h"
#include "GncLoc.h"
#include "GncArgs.h"
#include "CamArgs.h"
#include "../data/GNC.h"
#include "../thread/Camera.h"
#include "../thread/GncRecvByNet.h"
#include "../data/ImageData.h"
#include "../thread/CamCtrlByNet.h"
#include "../data/DataInjection.h"
#include "../thread/CamCtrlByRs422.h"
#include "../thread/GncRecvByRs422.h"
#include "../thread/GncParser.h"
#include "../thread/CamCtrlParser.h"
#include "../thread/SendImage.h"
#include "../value/values.h"

class Home : public wxFrame {
public:
    Home(const wxString &title, const wxPoint &pos, const wxSize &size);

    inline void set_image(const ImageData& _image) {
        images.emplace(_image);
    }

    inline void set_ctrl(DataInjection &_ctrl) {
        camera_ctrl_contents = _ctrl;
    }

    inline bool set_pos(int _pos) {
        if (pos_lock.TryLock() == wxMUTEX_BUSY) {
            return false;
        }
        pos = _pos;
        pos_lock.Unlock();
        return true;
    }

    inline void update_send_image() {
        if (!test_window_opened) {
            return;
        }
        test_window->set_recv_image(send_image);
        auto *event = new wxThreadEvent(wxEVT_THREAD, ID_SENDING_THREAD_UPDATE_TEST);
        wxQueueEvent(test_window->GetEventHandler(), event);
    }

    inline void set_test_window_state(bool state) {
        test_window_opened = state;
    }

    inline void set_gnc_loc_window_state(bool state) {
        gnc_loc_window_opened = state;
    }

    inline void set_gnc_arg_window_state(bool state) {
        gnc_arg_window_opened = state;
    }

    inline void set_cam_arg_window_state(bool state) {
        cam_arg_window_opened = state;
    }

    inline bool get_test_window_state() const {
        return test_window_opened;
    }

    inline bool get_gnc_loc_window_state() const {
       return gnc_loc_window_opened;
    }

    inline bool get_gnc_arg_window_state() const {
        return gnc_arg_window_opened;
    }

    inline bool get_cam_arg_window_state() const {
        return cam_arg_window_opened;
    }


    inline bool read_pos(int &_pos) {
        if (pos_lock.TryLock() == wxMUTEX_BUSY) {
            return false;
        }
        _pos = pos;
        pos_lock.Unlock();
        return true;
    }

private:
    void init_mars();
    void init_menu();
    void init_toolbar();
    void init_statusbar();
    void init_thread();
    void clean();
    void clean_global_values();

    void on_close(wxCloseEvent& event);
    void on_btn_start(wxCommandEvent& event);
    void on_btn_close(wxCommandEvent& event);
    void on_btn_test(wxCommandEvent& event);
    void on_btn_gnc(wxCommandEvent& event);
    void on_btn_loc(wxCommandEvent& event);
    void on_btn_args(wxCommandEvent& event);

    void on_see_mode(wxCommandEvent& event);
    void on_switch_saving_mode(wxCommandEvent& event);

    void on_update_statusbar();

    void on_update_ctrl(wxThreadEvent &event);
    void on_update_image(wxThreadEvent &event);
    void on_update_sending(wxThreadEvent &event);

    void on_menu_info(wxCommandEvent &event);
    void on_menu_help(wxCommandEvent &event);

    void on_menu_setting_ip_this(wxCommandEvent &event);
    void on_menu_setting_ip_remote(wxCommandEvent &event);
    void on_menu_setting_port_gnc(wxCommandEvent &event);
    void on_menu_setting_port_dij(wxCommandEvent &event);
    void on_menu_setting_port_image(wxCommandEvent &event);


    wxString get_work_mode_text();
    wxString get_image_mode_text();


private:
    int pos = -1;

    bool camera_is_start = false;
    bool test_window_opened = false;
    bool gnc_loc_window_opened = false;
    bool gnc_arg_window_opened = false;
    bool cam_arg_window_opened = false;


    wxImage send_image;
    wxMenu *base, *setting, *help;
    wxMenuBar *menu;
    wxButton *btn_exit, *btn_loc, *btn_check, *btn_args, *btn_gnc;
    wxToolBar *tool_bar;
    wxStatusBar *status_bar;
    wxGauge * send_gauge;
    wxRadioBox* saving_mode, *always_see;
    wxImagePanel *mars_img;

    wxMutex pos_lock;

    // thead;
    Camera *camera = nullptr;
    GncRecvByNet *gnc_receiver = nullptr;
    GncParser *gnc_parser = nullptr;
    GncRecvByRs422 *gnc_rs422_receiver = nullptr;
    CamCtrlParser *ctrl_parser = nullptr;
    CamCtrlByNet * ctrl_receiver = nullptr;
    CamCtrlByRs422 *ctrl_rs422_receiver = nullptr;
    SendImage *sender;

    // poped windows
    CamArgs *camera_args_window;
    Test *test_window;
    GncArgs *gnc_args_window;
    GncLoc *gnc_loc_window;

    // data for update ui
    std::queue<ImageData> images;
    DataInjection camera_ctrl_contents;

private:
DECLARE_EVENT_TABLE()
};


#endif //MDPI_CAMERA_HOME_H
