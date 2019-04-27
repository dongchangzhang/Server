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

    void OnQuit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void OnStart(wxCommandEvent &event);

    void OnStop(wxCommandEvent &event);

    void init_variables();
    void add_wins_into_sizer();

public:
    static MyImgPanel *img;
    static wxTextCtrl *tc1, *tc2, *tc3;

private:
    bool threadRunning = false;

    wxMenuBar *menuBar;
    wxButton *start;
    wxMenu *fileMenu, *helpMenu;
    wxBoxSizer *sizerAll, *sizerLog, *sizerBtn, *sizerImg;
    wxStaticText *gncLogTitle, *imgLogTitle, *cmdLogTitle;

private:
    enum {
        Btn_Stop,
        Btn_Start,
    };
    DECLARE_EVENT_TABLE()
};


#endif //SERVERGUI_MYFRAME_H
