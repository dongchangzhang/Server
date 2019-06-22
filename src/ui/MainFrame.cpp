//
// Created by z on 19-4-26.
//

#include "MainFrame.h"
#include "../net/Server.h"
#include "../net/Client.h"
#include "ImageWorker.h"
#include "GncWorker.h"
#include "../utils/utils.h"
#include <iostream>


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(Btn_Start, MainFrame::OnStart)
                EVT_BUTTON(Btn_Stop, MainFrame::OnStop)
                EVT_BUTTON(Btn_Pause, MainFrame::OnPause)
                EVT_BUTTON(Btn_Recv, MainFrame::OnRecv)
                EVT_BUTTON(Btn_Output, MainFrame::OnOutput)
                EVT_THREAD(kThreadUpdateId, MainFrame::ThreadUpdate)
END_EVENT_TABLE()

void start_info_helper(MainFrame *frame) {
    Server server(INFO_PORT);
    float ratio = 0;
    u_char buf[4];
    auto *e0 = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    e0->SetInt(LOAD_ID);
    wxQueueEvent(frame->GetEventHandler(), e0);
    frame->dataLoad = true;
    while (ratio < 1) {
        server.recv(buf, 4);
        frame->dataLoad = false;
        memcpy(&ratio, buf, 4);
        frame->ratio = ratio;
        auto *e_tmp = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
        e_tmp->SetInt(LOAD_ID);
        wxQueueEvent(frame->GetEventHandler(), e_tmp);
    }
    frame->ratio = 1;
    frame->dataLoad = true;

    auto *e1 = new wxThreadEvent(wxEVT_THREAD, kThreadUpdateId);
    e1->SetInt(LOAD_ID);
    wxQueueEvent(frame->GetEventHandler(), e1);
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

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    CreateStatusBar(1);
    SetStatusText(wxT("请首先启动相机，然后启动本服务器。等待启动中..."));
    init_variables();
    add_wins_into_sizer();
    Center();
}

void MainFrame::init_variables() {
    // sizer
    panel = new wxPanel(this);

    rotate_sizer = new wxBoxSizer(wxHORIZONTAL);
    error_sizer = new wxBoxSizer(wxHORIZONTAL);
    sun_sizer = new wxBoxSizer(wxHORIZONTAL);

    camera_state_sizer = new wxBoxSizer(wxHORIZONTAL);
    window_sizer = new wxBoxSizer(wxHORIZONTAL);
    expo_sizer = new wxBoxSizer(wxHORIZONTAL);

    output_sizer = new wxBoxSizer(wxHORIZONTAL);
    btn_sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer_all = new wxBoxSizer(wxVERTICAL);
    gnc_sizer = new wxBoxSizer(wxVERTICAL);
    camera_sizer = new wxBoxSizer(wxVERTICAL);

    // gnc
    gnc_title = new wxStaticText(panel, wxID_ANY, _T("* GNC参数设置"));

    rotate_title = new wxStaticText(panel, wxID_ANY, _T("相机旋转角度（度）：pitch "));
    yaw_title = new wxStaticText(panel, wxID_ANY, _T(" yaw "));
    roll_title = new wxStaticText(panel, wxID_ANY, _T(" roll "));
    t_pitch = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_yaw = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_roll = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    error_title = new wxStaticText(panel, wxID_ANY, _T("旋转角度误差（度）：pitch "));
    error_yaw_title = new wxStaticText(panel, wxID_ANY, _T(" yaw "));
    error_roll_title = new wxStaticText(panel, wxID_ANY, _T(" roll "));
    t_error_pitch = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_error_yaw = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_error_roll = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    sun_title = new wxStaticText(panel, wxID_ANY, _T("太阳方向"));
    t_sun_direct = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    // camera
    camera_title = new wxStaticText(panel, wxID_ANY, _T("* 相机参数设置"));
    open_win_title = new wxStaticText(panel, wxID_ANY, _T("开窗设置：起始行号 "));
    j_title = new wxStaticText(panel, wxID_ANY, _T(" 起始列号 "));
    ni_title = new wxStaticText(panel, wxID_ANY, _T(" 开窗行数 "));
    nj_title = new wxStaticText(panel, wxID_ANY, _T(" 开窗列数 "));
    t_start_i = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_start_j = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_ni = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_nj = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    expo_title = new wxStaticText(panel, wxID_ANY, _T("相机曝光时间 （秒）"));
    t_expo = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    radio_taking = new wxRadioButton(panel, Radio_Taking, wxT("拍照模式"));
    radio_pausing = new wxRadioButton(panel, Radio_Pausing, wxT("停机模式"));
    radio_taking->SetValue(true);

    // local
    gauge = new wxGauge(panel, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
    gauge->SetValue(0);

    output_title = new wxStaticText(panel, wxID_ANY, _T("* 选择图片保存路径"));
    dir_dialog = new wxDirDialog(panel, wxT("选择图片输出路径"));
    btn_output_path = new wxButton(panel, Btn_Output, wxT("选择"));

    btn_stop = new wxButton(panel, Btn_Stop, wxT("终止"));
    btn_start = new wxButton(panel, Btn_Start, wxT("启动"));
    btn_pause = new wxButton(panel, Btn_Pause, wxT("暂停"));
    btn_onece = new wxButton(panel, Btn_Recv, wxT("更新相机参数"));

    // space
    space_1 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_2 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_3 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_4 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_5 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);

    line_1 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_2 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_3 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_4 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);

    t_output_path = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
}

void MainFrame::add_wins_into_sizer() {

    rotate_sizer->Add(rotate_title);
    rotate_sizer->Add(t_pitch);
    rotate_sizer->Add(yaw_title);
    rotate_sizer->Add(t_yaw);
    rotate_sizer->Add(roll_title);
    rotate_sizer->Add(t_roll);

    error_sizer->Add(error_title);
    error_sizer->Add(t_error_pitch);
    error_sizer->Add(error_yaw_title);
    error_sizer->Add(t_error_yaw);
    error_sizer->Add(error_roll_title);
    error_sizer->Add(t_error_roll);

    sun_sizer->Add(sun_title);
    sun_sizer->Add(t_sun_direct);

    gnc_sizer->Add(sun_sizer, 1, wxALL, 2);
    gnc_sizer->Add(rotate_sizer, 1, wxALL, 2);
    gnc_sizer->Add(error_sizer, 1, wxALL, 2);

    expo_sizer->Add(expo_title);
    expo_sizer->Add(t_expo);

    window_sizer->Add(open_win_title);
    window_sizer->Add(t_start_i);
    window_sizer->Add(j_title);
    window_sizer->Add(t_start_j);
    window_sizer->Add(ni_title);
    window_sizer->Add(t_ni);
    window_sizer->Add(nj_title);
    window_sizer->Add(t_nj);

    camera_state_sizer->Add(radio_taking, 3);
    camera_state_sizer->Add(radio_pausing, 3);

    camera_sizer->Add(camera_state_sizer, 1, wxALL , 2);
    camera_sizer->Add(expo_sizer, 1, wxALL , 2);
    camera_sizer->Add(window_sizer, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 2);

    output_sizer->Add(t_output_path, 4);
    output_sizer->Add(btn_output_path, 1);

    btn_sizer->Add(btn_onece, 3);
    btn_sizer->Add(space_1, 1);
    btn_sizer->Add(btn_start, 3);
    btn_sizer->Add(space_2, 1);
    btn_sizer->Add(btn_pause, 3);
    btn_sizer->Add(space_3, 1);
    btn_sizer->Add(btn_stop, 3);
    btn_sizer->Add(space_4, 1);

    sizer_all->Add(gauge, 0, wxEXPAND, 10);

    sizer_all->Add(line_1, 0, wxEXPAND | wxALL, 2);

    sizer_all->Add(gnc_title, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(gnc_sizer, 2, wxALIGN_CENTER);

    sizer_all->Add(line_2, 0, wxEXPAND | wxALL, 2);

    sizer_all->Add(camera_title, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(camera_sizer, 2, wxALIGN_CENTER);

    sizer_all->Add(line_3, 0, wxEXPAND | wxALL, 10);

    sizer_all->Add(output_title, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(output_sizer, 1, wxALIGN_CENTER);

    sizer_all->Add(line_4, 0, wxEXPAND | wxALL, 10);

    sizer_all->Add(btn_sizer, 1, wxALIGN_CENTER);

    panel->SetSizer(sizer_all);
}

void MainFrame::OnStart(wxCommandEvent &event) {
    // send auto mode cmd to mars camera
    wxCommandEvent tmp;
    OnAutoMode(tmp);

    // start data receive state thread
    std::thread th0 = std::thread(start_info_helper, this);
    th0.detach();

    // recv image and send gnc threads
    auto *th1 = new GncWorker(this, IP, GNC_PORT);
    auto *th2 = new ImageWorker(this, SEND_IMAGE_PORT);
    // start threads
    th1->start_thread();
    th2->start_thread();

    btn_pause->Enable();
    btn_start->Disable();

    threadRunning = true;
    SetStatusText(wxT("Server Started"));
}
void MainFrame::OnStop(wxCommandEvent &event) {
    Close(true);
}
void MainFrame::OnOutput(wxCommandEvent &event) {
    dir_dialog->ShowModal();
    output_path = dir_dialog->GetPath();
    t_output_path->SetValue(output_path);
}
void MainFrame::OnPause(wxCommandEvent &event) {
    if (threadRunning) {
        SetStatusText(wxT("Server had been Started, not need to restart!"));
        return;
    }
    wxCommandEvent tmp;
    OnAutoMode(tmp);
    btn_start->SetLabelText(_T("系统正在运行，无需重复点击"));
    std::thread th0 = std::thread(start_info_helper, this);
    th0.detach();
    auto *th1 = new GncWorker(this, IP, GNC_PORT);
    auto *th2 = new ImageWorker(this, SEND_IMAGE_PORT);
    th1->start_thread();
    th2->start_thread();
    threadRunning = true;

    btn_pause->Disable();
    btn_start->Enable();

    SetStatusText(wxT("Server Started"));
}

void MainFrame::ThreadUpdate(wxThreadEvent &event) {
    int id = event.GetInt();
    if (id == IMG_ID) {
        photo_update();
    } else if (id == GNC_ID) {
        gnc_update();
    } else if (id == LOAD_ID) {
        load_data();
    }
}

void MainFrame::photo_update() {
    if (mphoto.cols == 0 && mphoto.rows == 0) {
        mphoto = cv::Mat(3072, 4096, CV_8UC3, cv::Scalar::all(0));
    }
    gauge->SetValue(ratio * 100 > 100 ? 100 : ratio * 100);

    if (ratio == 0) {
        gaugeTitle->SetLabelText(_T("正在接收图片数据..."));
        char info[128];
    } else if (ratio == 1) {
//        this->img->update(mphoto);
        gaugeTitle->SetLabelText(_T("图片数据接收完成!"));
    }
    Refresh();
}

void MainFrame::gnc_update() {
//    img2->draw(yy, zz);
    Refresh();
}

void MainFrame::load_data() {
    if (ratio < 1) {
        gaugeTitle->SetLabelText(_T("相机正在载入图片数据..."));
        gauge->SetValue(ratio * 100);
    } else {
        gaugeTitle->SetLabelText(_T("相机载入图片数据完成"));
        gauge->SetValue(100);
    }
}

void MainFrame::OnRecv(wxCommandEvent &event) {
    if (autoModeFlag)
        return;
    char cmd = 0x00;
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();
    gaugeTitle->SetLabelText(_T("正在发送指令..."));
    gauge->SetValue(0);
}

void MainFrame::OnAutoMode(wxCommandEvent &event) {
    std::cout << "auto mode" << std::endl;
    btn_onece->Disable();
    char cmd = 0x11;
    autoModeFlag = true;
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();
    std::thread th1 = std::thread(cmd_helper, cmd);
    th1.detach();

    SetStatusText(wxT("设置为自动模式，此时相机自动发送图片！"));
}

void MainFrame::OnManualMode(wxCommandEvent &event) {
    std::cout << "manual mode" << std::endl;
    char cmd = 0x22;
    autoModeFlag = false;
    btn_onece->Enable();
    std::thread th = std::thread(cmd_helper, cmd);
    th.detach();

    std::thread th1 = std::thread(cmd_helper, cmd);
    th1.detach();
    SetStatusText(wxT("设置为手动模式，需要手动点击按钮才能接收图片！"));
}
