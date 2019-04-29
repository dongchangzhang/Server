//
// Created by z on 19-4-26.
//

#include "MyFrame.h"
#include "../net/Server.h"
#include "../net/Client.h"
#include "MyRecvThread.h"
#include "MyGNCThread.h"
#include "../utils/utils.h"
#include <iostream>


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_BUTTON(Btn_Start, MyFrame::OnStart)
                EVT_THREAD(kThreadUpdateId, MyFrame::ThreadUpdate)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    fileMenu = new wxMenu;
    helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
                     wxT("Show about dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
                     wxT("Quit this program"));

    CreateStatusBar(2);
    SetStatusText(wxT("Mars Server"));
    init_variables();
    add_wins_into_sizer();
}

void MyFrame::init_variables() {
    img = new MyImgPanel(this, wxT("0.png"), wxBITMAP_TYPE_PNG);
    sizerLog = new wxBoxSizer(wxVERTICAL);
    sizerBtn = new wxBoxSizer(wxHORIZONTAL);
    sizerImg = new wxBoxSizer(wxHORIZONTAL);
    sizerAll = new wxBoxSizer(wxVERTICAL);
    gncLogTitle = new wxStaticText(this, wxID_ANY, _T("GNC Log:"));
    imgLogTitle = new wxStaticText(this, wxID_ANY, _T("IMG Log:"));
    cmdLogTitle = new wxStaticText(this, wxID_ANY, _T("CMD Log:"));
    tc1 = new wxTextCtrl(this, wxID_ANY, "",
                         wxDefaultPosition, wxDefaultSize,
                         wxTE_MULTILINE | wxTE_READONLY);
    tc2 = new wxTextCtrl(this, wxID_ANY, "",
                         wxDefaultPosition, wxDefaultSize,
                         wxTE_MULTILINE | wxTE_READONLY);
    tc3 = new wxTextCtrl(this, wxID_ANY, "",
                         wxDefaultPosition, wxDefaultSize,
                         wxTE_MULTILINE | wxTE_READONLY);
    start = new wxButton(this, Btn_Start, wxT("Start"));
}

void MyFrame::add_wins_into_sizer() {
    sizerLog->Add(gncLogTitle);
    sizerLog->Add(tc1, 1, wxEXPAND | wxALL, 5);
    sizerLog->Add(imgLogTitle);
    sizerLog->Add(tc2, 1, wxEXPAND | wxALL, 5);
    sizerLog->Add(cmdLogTitle);
    sizerLog->Add(tc3, 1, wxEXPAND | wxALL, 5);
    sizerBtn->Add(start, 1, wxEXPAND | wxALL, 2);
    sizerImg->Add(img, 3, wxEXPAND | wxCENTER | wxALL, 10);
    sizerImg->Add(sizerLog, 2, wxEXPAND | wxALL, 10);
    sizerAll->Add(sizerBtn, 0, wxEXPAND | wxALL, 10);
    sizerAll->Add(sizerImg, 2, wxEXPAND | wxALL, 10);
    this->SetSizer(sizerAll);
}

void MyFrame::OnStart(wxCommandEvent &event) {
    if (threadRunning) {
        SetStatusText(wxT("Server had been Started, not need to restart!"));
        return;
    }
    threadRunning = true;
    auto *th1 = new MyGNCThread(this, IP, GNC_PORT);
    auto *th2 = new MyRecvThread(this, SEND_IMAGE_PORT);
    th1->start_thread();
    th2->start_thread();
    SetStatusText(wxT("Server Started"));
}

void MyFrame::ThreadUpdate(wxThreadEvent &event) {
    int id = event.GetInt();
    if (id == IMG_ID)
        photo_update();
    else if (id == GNC_ID)
        gnc_update();
}

void MyFrame::photo_update() {
    char buf[128];
    snprintf(buf, 128, "HAHA %d", haha++);
    this->img->update(mphoto);
    SetStatusText(buf);
    Refresh();
}

void MyFrame::gnc_update() {
    int id = 0, count = 0;
    char infobuf[128];
    short y, m, d, hh, mm, ss;
    get_time(y, m, d, hh, mm, ss);
    snprintf(infobuf, 128, "%4d/%2d/%2d %2d:%2d:%2d -> send gnc %5d, %4d bits\n", y, m, d, hh, mm, ss, id, count);
    this->tc1->WriteText(infobuf);
    Refresh();
}

