//
// Created by vhdsih on 2019-06-15.
//

#ifndef PICTURECOLLAGEMAKER_MAINFRAME_H
#define PICTURECOLLAGEMAKER_MAINFRAME_H


#include "Image.h"
#include "../conf/constants.h"
#include "../model/data/ImageData.h"
#include "Constructer.h"

#include <string>
#include <iostream>

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statline.h>


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    inline void set_now(int _now) {
        now = _now;
    }
    inline void set_all(int _all) {
        all = _all;
    }
    inline void set_ratio(float _ratio) {
        ratio = _ratio;
    }
    inline void set_canvas(ImageData &_canvas) {
        canvas = _canvas;
    }
    inline std::string get_image_path() {
        return images_path;
    }
    inline void set_index(int _idx) {
        index = _idx;
    }

private:
    void End(wxCommandEvent &event);
    void Begin(wxCommandEvent &event);
    void Save(wxCommandEvent &event);
    void Lead(wxCommandEvent &event);
    void Close(wxCloseEvent &event);

    void ThreadUpdate(wxThreadEvent& event);

    inline void GetImageDir(wxCommandEvent &event);
    inline void GetOutputDir(wxCommandEvent &event);

    bool check_path_passed();
    void init_toolbar();
    void init_statusbar();


private:
    int index = 0;
    bool is_lead = false;
    bool is_running = false;
    bool is_finished = false;

    float ratio = 0;

    ImageData canvas;

    std::string images_path, output_path, output_file;

    int now, all;
    wxImagePanel *mars_image;

    wxGauge *gauge;

    wxDirDialog *dir_dialog;

    wxTextCtrl *image_dir_opt, *output_dir_opt, *interval;

    wxStaticText *path;

    wxButton *btn_end, *btn_begin, *btn_get_image_dir, *btn_get_output_dir, *btn_save, *btn_lead;

    wxToolBar *tool_bar;
    wxStatusBar *status_bar;

    Constructer *th;

private:
DECLARE_EVENT_TABLE()
};


inline void MainFrame::GetImageDir(wxCommandEvent &event) {
    dir_dialog->ShowModal();
    images_path = dir_dialog->GetPath();
    image_dir_opt->SetValue(images_path);
}

inline void MainFrame::GetOutputDir(wxCommandEvent &event) {
    dir_dialog->ShowModal();
    output_path = dir_dialog->GetPath();
    output_dir_opt->SetValue(output_path);
}
#endif //PICTURECOLLAGEMAKER_MAINFRAME_H
