//
// Created by z on 19-4-26.
//

#include "MyFrame.h"
#include "../net/Server.h"
#include "../net/Client.h"
#include <iostream>

MyImgPanel *MyFrame::img;
wxTextCtrl *MyFrame::tc1, *MyFrame::tc2, *MyFrame::tc3;

static void get_time(short &y, short &m, short &d, short &hh, short &mm, short &ss) {
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    y = ptminfo->tm_year + 1900;
    m = ptminfo->tm_mon;
    d = ptminfo->tm_mday;
    hh = ptminfo->tm_hour;
    mm = ptminfo->tm_min;
    ss = ptminfo->tm_sec;
}

static void gnc(MyFrame *frame, std::string ip, int port) {
    int count = 0, id = 0;
    bool stop = false;
    short y, m, d, hh, mm, ss;
    Client client(ip, port);
    std::chrono::milliseconds dura(2500);
    std::chrono::milliseconds dura_long(500);
    char infobuf[128];
    while (!stop) {
        ++id;
        while ((count = client.send_gnc()) == -1) {
            std::this_thread::sleep_for(dura);
        }

        get_time(y, m, d, hh, mm, ss);
        snprintf(infobuf, 128, "%4d/%2d/%2d %2d:%2d:%2d -> send gnc %5d, %4d bits\n", y, m, d, hh, mm, ss, id, count);
        frame->tc1->WriteText(infobuf);
        std::this_thread::sleep_for(dura_long);
    }
}

static void photo(MyFrame *frame, int port) {
    int id = 0, count = 0;
    bool stop = false;
    short y, m, d, hh, mm, ss;
    char infobuf[128];
    Server server(port);
    std::chrono::milliseconds dura(500);
    while (!stop) {
        ++id;
        server.recv_photo();
//        if (id % 5 == 0)
//        frame->img->update(wxT("1.png"), wxBITMAP_TYPE_PNG);
//        if (id % 5 == 1)
//            frame->img->update(wxT("2.png"), wxBITMAP_TYPE_PNG);
//
//        if (id % 5 == 2)
//            frame->img->update(wxT("3.png"), wxBITMAP_TYPE_PNG);
//        if (id % 5 == 3)
//            frame->img->update(wxT("4.png"), wxBITMAP_TYPE_PNG);
//        if (id % 5 == 4)
//            frame->img->update(wxT("5.png"), wxBITMAP_TYPE_PNG);

        get_time(y, m, d, hh, mm, ss);
        snprintf(infobuf, 128, "%4d/%2d/%2d %2d:%2d:%2d -> starting to recv photo\n", y, m, d, hh, mm, ss);
        frame->tc2->WriteText(infobuf);
        std::this_thread::sleep_for(dura);
    }
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
                EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
                EVT_BUTTON(Btn_Start, MyFrame::OnStart)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    fileMenu = new wxMenu;
    helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
                     wxT("Show about dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
                     wxT("Quit this program"));
    menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));
    SetMenuBar(menuBar);
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
void MyFrame::OnAbout(wxCommandEvent &event) {
    wxString msg;
    msg.Printf(wxT("hello"));
    wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnQuit(wxCommandEvent &event) {
    Close();
}

void MyFrame::OnStart(wxCommandEvent &event) {
    if (threadRunning) {
        SetStatusText(wxT("Server had been Started, not need to restart!"));
        return;
    }
    threadRunning = true;
    std::thread th1(gnc, this, IP, GNC_PORT);
    std::thread th2(photo, this, SEND_IMAGE_PORT);
    th1.detach();
    th2.detach();
    SetStatusText(wxT("Server Started"));
}


