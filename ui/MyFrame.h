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
#include "wx/statline.h"


class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnStart(wxCommandEvent &event);
    void OnRecv(wxCommandEvent &event);
    void OnAutoMode(wxCommandEvent &event);
    void OnManualMode(wxCommandEvent &event);

    void ThreadUpdate(wxThreadEvent& event);

    void photo_update();
    void gnc_update();
    void load_data();

    void init_variables();
    void add_wins_into_sizer();

public:
    enum {
        Btn_Start,
        Btn_Auto,
        Btn_Manual,
        Btn_Recv,
    };
    cv::Mat mphoto;
    int nframe = 0;
    char gncinfo[512], photoinfo[128];
    wxString gncInfo;
    wxGauge *gauge;
    wxStaticText *gaugeTitle;
    bool dataLoad = true;

    double yy, zz;

    // photo gauge
    float ratio = 0;
    int line_nus = 0;

private:
    int haha = 0;
    bool threadRunning = false;

    MyImgPanel *img, *img2;
    wxTextCtrl *tc1, *tc2;

    wxButton *start, *sendGnc;
    wxRadioButton *autoMode, *manualMode;
    wxBoxSizer *sizerAll, *sizerCmd;
    wxStaticText *gncImgTitle, *photoTitle, *modeTitle, *gncTitle;
    wxStaticLine* staticLine1, * staticLine2, *staticLine3, *staticLine4, *staticLine5;

private:
    DECLARE_EVENT_TABLE()
};

#endif //SERVERGUI_MYFRAME_H
