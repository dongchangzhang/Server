//
// Created by z on 19-4-26.
//

#ifndef SERVERGUI_MYFRAME_H
#define SERVERGUI_MYFRAME_H


#include <wx/wx.h>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <condition_variable>

#include "../constants.h"
#include "MyImgPanel.h"


class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnStart(wxCommandEvent &event);
    void ThreadUpdate(wxThreadEvent& event);

    void photo_update();
    void gnc_update();

    void init_variables();
    void add_wins_into_sizer();

public:
    enum {
        Btn_Start,
    };
    cv::Mat mphoto;
    char gncinfo[128], photoinfo[128];

private:
    int haha = 0;
    bool threadRunning = false;

    MyImgPanel *img;
    wxTextCtrl *tc1, *tc2, *tc3;

    wxButton *start;
    wxMenu *fileMenu, *helpMenu;
    wxBoxSizer *sizerAll, *sizerLog, *sizerBtn, *sizerImg;
    wxStaticText *gncLogTitle, *imgLogTitle, *cmdLogTitle;

private:
    DECLARE_EVENT_TABLE()
};

#endif //SERVERGUI_MYFRAME_H
