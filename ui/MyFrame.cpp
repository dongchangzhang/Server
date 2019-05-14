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
                EVT_BUTTON(Btn_Recv, MyFrame::OnRecv)
                EVT_RADIOBUTTON(Btn_Auto, MyFrame::OnAutoMode)
                EVT_RADIOBUTTON(Btn_Manual, MyFrame::OnManualMode)
                EVT_THREAD(kThreadUpdateId, MyFrame::ThreadUpdate)
END_EVENT_TABLE()

void start_info_helper(MyFrame *frame) {
    Server server(INFO_PORT);
    float ratio = 0;
    u_char buf[4];
    auto *e0 = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    e0->SetInt(LOAD_ID);
    wxQueueEvent(frame->GetEventHandler(), e0);
    e0->UnRef();
    frame->dataLoad = true;
    while (ratio < 1) {
        server.recv(buf, 4);
        frame->dataLoad = false;
        memcpy(&ratio, buf, 4);
        frame->ratio = ratio;
        auto *e_tmp = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        e_tmp->SetInt(LOAD_ID);
        wxQueueEvent(frame->GetEventHandler(), e_tmp);
        e_tmp->UnRef();
    }
    frame->ratio = 1;
    frame->dataLoad = true;
    auto *e1 = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    e1->SetInt(LOAD_ID);
    wxQueueEvent(frame->GetEventHandler(), e1);
    e1->UnRef();
}

void cmd_helper(char mode) {
    Client client(IP, CMD_PORT);
    char buf[2];
    buf[0] = mode;
    buf[1] = '\0';
    while ((client.send(buf, 1)) == -1) {
        wxMilliSleep(100);
    }
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    CreateStatusBar(1);
    SetStatusText(wxT("请首先启动相机，然后启动本服务器。等待启动中..."));
    init_variables();
    add_wins_into_sizer();
}

void MyFrame::init_variables() {
    img = new MyImgPanel(this);
    img2 = new MyImgPanel(this, 255);
    staticLine1 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxSize(150, -1), wxLI_HORIZONTAL);
    staticLine2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxSize(150, -1), wxLI_HORIZONTAL);
    staticLine3 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxSize(150, -1), wxLI_HORIZONTAL);
    staticLine4 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxSize(150, -1), wxLI_HORIZONTAL);
    staticLine5 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxSize(150, -1), wxLI_HORIZONTAL);

//    sizerLog = new wxBoxSizer(wxVERTICAL);
    sizerCmd = new wxBoxSizer(wxVERTICAL);
    sizerAll = new wxBoxSizer(wxHORIZONTAL);
    modeTitle = new wxStaticText(this, wxID_ANY, _T("切换图片传输方式"));
    photoTitle = new wxStaticText(this, wxID_ANY, _T("图片信息"));
    gncTitle = new wxStaticText(this, wxID_ANY, _T("GNC轨道信息"));
    gncImgTitle = new wxStaticText(this, wxID_ANY, _T("相机位置实时状态图"));
    gaugeTitle = new wxStaticText(this, wxID_ANY, _T("信息条"));
    tc1 = new wxTextCtrl(this, wxID_ANY, "",
                         wxDefaultPosition, wxDefaultSize,
                         wxTE_MULTILINE | wxTE_READONLY);
    tc2 = new wxTextCtrl(this, wxID_ANY, "",
                         wxDefaultPosition, wxDefaultSize,
                         wxTE_READONLY);
    gauge = new wxGauge(this, wxID_ANY,
                        100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
    // test
    gauge->SetValue(0);

    start = new wxButton(this, Btn_Start, wxT("点击此按钮启动系统"));
    sendGnc = new wxButton(this, Btn_Recv, wxT("手动模式点击接收图片"));
    autoMode = new wxRadioButton(this, Btn_Auto, wxT("自动模式"));
    manualMode = new wxRadioButton(this, Btn_Manual, wxT("手动模式"));
}

void MyFrame::add_wins_into_sizer() {
    short ww = 2;
    sizerAll->Add(img, 8, wxEXPAND | wxALL, ww);
    sizerCmd->Add(start, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(staticLine1, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(gaugeTitle, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(gauge, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(staticLine2, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(modeTitle, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(autoMode, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(manualMode, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(sendGnc, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(staticLine3, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(photoTitle, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(tc2, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(staticLine4, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(gncTitle, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(tc1, 1, wxEXPAND | wxALL, ww);
    sizerCmd->Add(staticLine5, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(gncImgTitle, 0, wxEXPAND | wxALL, ww);
    sizerCmd->Add(img2, 2, wxEXPAND | wxALL, ww);

    sizerAll->Add(sizerCmd, 3, wxEXPAND | wxALL, ww);
    this->SetSizer(sizerAll);
}

void MyFrame::OnStart(wxCommandEvent &event) {
    if (threadRunning) {
        SetStatusText(wxT("Server had been Started, not need to restart!"));
        return;
    }
    wxCommandEvent tmp;
    OnAutoMode(tmp);
    start->SetLabelText(_T("系统正在运行，无需重复点击"));
    std::thread th0 = std::thread(start_info_helper, this);
    th0.detach();
    auto *th1 = new MyGNCThread(this, IP, GNC_PORT);
    auto *th2 = new MyRecvThread(this, SEND_IMAGE_PORT);
    th1->start_thread();
    th2->start_thread();
    threadRunning = true;
    SetStatusText(wxT("Server Started"));
}

void MyFrame::ThreadUpdate(wxThreadEvent &event) {
    int id = event.GetInt();
    if (id == IMG_ID)
        photo_update();
    else if (id == GNC_ID)
        gnc_update();
    else if (id == LOAD_ID)
        load_data();
}

void MyFrame::photo_update() {
    this->img->update(mphoto);
    gauge->SetValue(ratio * 100);
    if (ratio == 0) {
        gaugeTitle->SetLabelText(_T("正在接收图片数据..."));
        char info[128];
        this->tc2->Clear();
        this->tc2->WriteText(wxString::Format(_T("图像编号（模255后）: %d"), nframe));
    } else if (ratio == 1) {
        gaugeTitle->SetLabelText(_T("图片数据接收完成!"));
    }
    Refresh();
}

void MyFrame::gnc_update() {
    this->tc1->Clear();
    tc1->WriteText(gncInfo);
    img2->draw(yy, zz);
    Refresh();
}

void MyFrame::load_data() {
    if (ratio < 1) {
        gaugeTitle->SetLabelText(_T("相机正在载入图片数据..."));
        gauge->SetValue(ratio * 100);
    } else {
        gaugeTitle->SetLabelText(_T("相机载入图片数据完成"));
        gauge->SetValue(100);
    }
}

void MyFrame::OnRecv(wxCommandEvent &event) {
    char cmd = 0x00;
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();
    gaugeTitle->SetLabelText(_T("正在发送指令..."));
    gauge->SetValue(0);
}

void MyFrame::OnAutoMode(wxCommandEvent &event) {
    char cmd = 0x11;
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();
    SetStatusText(wxT("设置为手动模式，此时相机自动发送图片！"));
}

void MyFrame::OnManualMode(wxCommandEvent &event) {
    char cmd = 0x22;
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();
    SetStatusText(wxT("设置为手动模式，需要手动点击按钮才能接收图片！"));
}
