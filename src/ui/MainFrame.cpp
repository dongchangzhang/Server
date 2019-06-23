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
                EVT_BUTTON(Btn_Camera, MainFrame::OnSend)
                EVT_BUTTON(Btn_Output, MainFrame::OnOutput)
                EVT_RADIOBUTTON(Radio_Taking, MainFrame::OnTaking)
                EVT_RADIOBUTTON(Radio_Pausing, MainFrame::OnPausing)
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

void dij_helper(MainFrame *frame) {
    Client client(IP, INJECTION_POER);
    uchar buffer[200];
    int len = frame->dij.get_dij_into_buffer(buffer);
    client.send((char*)buffer, len);
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
    image_mode_sizer = new wxBoxSizer(wxHORIZONTAL);

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

    info_0 = new wxStaticText(panel, wxID_ANY, _T("* 太阳方向设置为空，则默认相机方向和太阳方向相同；也可以输入太阳方向以测试光照下成像效果，格式为 dx dy dz"));

    // camera
    camera_title = new wxStaticText(panel, wxID_ANY, _T("* 相机参数设置"));
    open_win_title = new wxStaticText(panel, wxID_ANY, _T("开窗设置：起始行号"));
    j_title = new wxStaticText(panel, wxID_ANY, _T(" 起始列号 "));
    ni_title = new wxStaticText(panel, wxID_ANY, _T(" 开窗行数 "));
    nj_title = new wxStaticText(panel, wxID_ANY, _T(" 开窗列数 "));
    t_start_i = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_start_j = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_ni = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    t_nj = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    expo_title = new wxStaticText(panel, wxID_ANY, _T("最大曝光时间（ms）"));
    t_expo = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    image_mode_title = new wxStaticText(panel, wxID_ANY, _T("图像模式  1/2/3/4/5 "));
    t_image_mode = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    info_1 = new wxStaticText(panel, wxID_ANY, _T("* 图像模式：1为传送原始图像（4096x3072），2为传输2：1采样后图像，3为4：1采样，4为8:1, 5为开窗"));
    info_2 = new wxStaticText(panel, wxID_ANY, _T("* 只有图像模式为5时开窗参数才起作用，开窗尺寸需要满足图片尺寸：行号+开窗行数 < 3072; 列号+开窗列数 < 4096, 否则不开窗!"));

    radio_taking = new wxRadioButton(panel, Radio_Taking, wxT("拍照模式"));
    radio_pausing = new wxRadioButton(panel, Radio_Pausing, wxT("停机模式"));

    // local
    gauge = new wxGauge(panel, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);

    output_title = new wxStaticText(panel, wxID_ANY, _T("* 选择图片保存路径"));
    dir_dialog = new wxDirDialog(panel, wxT("选择图片输出路径"));
    btn_output_path = new wxButton(panel, Btn_Output, wxT("选择"));

    btn_stop = new wxButton(panel, Btn_Stop, wxT("终止"));
    btn_start = new wxButton(panel, Btn_Start, wxT("启动"));
    btn_pause = new wxButton(panel, Btn_Pause, wxT("暂停"));
    btn_onece = new wxButton(panel, Btn_Camera, wxT("更新相机参数"));

    // space
    space_1 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_2 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_3 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);
    space_4 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(0, 0), wxLI_HORIZONTAL);

    line_1 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_2 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_3 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);
    line_4 = new wxStaticLine(panel, wxID_STATIC, wxDefaultPosition, wxSize(150, 1), wxLI_HORIZONTAL);

    t_output_path = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    gauge->SetValue(0);
    t_pitch->SetValue("0");
    t_yaw->SetValue("0");
    t_roll->SetValue("0");
    t_error_pitch->SetValue("0");
    t_error_yaw->SetValue("0");
    t_error_roll->SetValue("0");
    radio_taking->SetValue(true);

    t_expo->SetValue("150");
    t_image_mode->SetValue("1");
    t_output_path->SetValue("./");
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

    image_mode_sizer->Add(image_mode_title);
    image_mode_sizer->Add(t_image_mode);

    camera_state_sizer->Add(radio_taking, 3);
    camera_state_sizer->Add(radio_pausing, 3);

    camera_sizer->Add(camera_state_sizer, 1, wxALL , 2);
    camera_sizer->Add(expo_sizer, 1, wxALL , 2);
    camera_sizer->Add(image_mode_sizer, 1, wxALL , 2);
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
    sizer_all->Add(info_0, 0, wxALL |wxALIGN_LEFT, 10);

    sizer_all->Add(line_2, 0, wxEXPAND | wxALL, 2);

    sizer_all->Add(camera_title, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(camera_sizer, 2, wxALIGN_CENTER);
    sizer_all->Add(info_1, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(info_2, 0, wxALL |wxALIGN_LEFT, 10);

    sizer_all->Add(line_3, 0, wxEXPAND | wxALL, 10);

    sizer_all->Add(output_title, 0, wxALL |wxALIGN_LEFT, 10);
    sizer_all->Add(output_sizer, 1, wxALIGN_CENTER);

    sizer_all->Add(line_4, 0, wxEXPAND | wxALL, 10);

    sizer_all->Add(btn_sizer, 1, wxALIGN_CENTER);

    panel->SetSizer(sizer_all);
}

void MainFrame::OnStart(wxCommandEvent &event) {

//     start data receive state thread
    std::thread th0 = std::thread(start_info_helper, this);
    th0.detach();

    // gnc
    std::string tmp(t_sun_direct->GetValue());
    std::istringstream is(tmp);
    if (is >> dx >> dy >> dz) {
        gnc = GNC(true);
        gnc.set_sun(dx, dy, dz);
    } else {
        gnc = GNC(false);
    }
    pitch = atof(t_pitch->GetValue());
    yaw = atof(t_yaw->GetValue());
    roll = atof(t_roll->GetValue());

    e_pitch = atof(t_error_pitch->GetValue());
    e_yaw = atof(t_error_yaw->GetValue());
    e_roll = atof(t_error_roll->GetValue());
    gnc.set_angle(pitch, yaw, roll);
    gnc.set_error(e_pitch, e_yaw, e_roll);


    // dij
    dij.set_work_mode(camera_running);
    dij.set_image_mode(atoi(t_image_mode->GetValue()));
    dij.set_window(atoi(t_start_i->GetValue()), atoi(t_start_j->GetValue()),
            atoi(t_ni->GetValue()), atoi(t_nj->GetValue()));
    dij.set_expo(atoi(t_expo->GetValue()));

    std::thread th_dij(dij_helper, this);
    th_dij.detach();


    // output
    output_path = t_output_path->GetValue();
    if (output_path.empty()) {
        output_path = "./";
    }

//     recv image and send gnc threads
    if (image_worker == nullptr || gnc_worker == nullptr) {
        auto *th1 = new GncWorker(this, IP, GNC_PORT);
        auto *th2 = new ImageWorker(this, SEND_IMAGE_PORT, output_path);
        // start threads
        th1->start_thread();
        th2->start_thread();
        image_worker = th1;
        gnc_worker = th2;
    }

    // disable
    btn_pause->Enable();
    btn_start->Disable();
    t_sun_direct->Disable();
    t_pitch->Disable();
    t_yaw->Disable();
    t_roll->Disable();
    t_error_pitch->Disable();
    t_error_yaw->Disable();
    t_error_roll->Disable();
    t_output_path->Disable();

    image_frame = new ImageFrame(wxT("实时结果_" + std::to_string(show_frame_id)),
                                 wxDefaultPosition, wxSize(512, 384 + 24), this);
    image_frame->SetBackgroundColour(wxColour(0, 0, 0));
    image_frame->Show(true);

    track_frame = new ImageFrame(wxT("轨道可视化_" + std::to_string(show_frame_id++)),
                                 wxDefaultPosition, wxSize(512, 384 + 24), this, 255);
    track_frame->SetBackgroundColour(wxColour(0, 0, 0));
    track_frame->Show(true);

    is_pause = false;
    image_frame_destroyed = false;
}
void MainFrame::OnStop(wxCommandEvent &event) {
    if (image_worker) {
        ((ImageWorker*)image_worker)->stop_thread();
    }
    if (gnc_worker) {
        ((GncWorker*)gnc_worker)->stop_thread();
    }
    Close(true);
}
void MainFrame::OnOutput(wxCommandEvent &event) {
    dir_dialog->ShowModal();
    output_path = dir_dialog->GetPath();
    t_output_path->SetValue(output_path);
}
void MainFrame::OnPause(wxCommandEvent &event) {
    is_pause = true;
    // enable
    btn_pause->Disable();
    btn_start->Enable();
    t_sun_direct->Enable();
    t_pitch->Enable();
    t_yaw->Enable();
    t_roll->Enable();
    t_error_pitch->Enable();
    t_error_yaw->Enable();
    t_error_roll->Enable();
    t_output_path->Enable();
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
        SetStatusText(_T("正在接收图片数据..."));
        char info[128];
    } else if (ratio == 1) {
        try {
            if (!image_frame_destroyed) {
                image_frame->img->update(mphoto);
            }
        } catch (cv::Exception & e) {
        } catch (...) {
        }

        SetStatusText(_T("接收图片数据完成"));
    }
    Refresh();
}

void MainFrame::gnc_update() {
    try {
        if (!image_frame_destroyed) {
            track_frame->img->draw(yy, zz);
        }
    } catch (cv::Exception & e) {
    } catch (...) {
    }
    Refresh();
}

void MainFrame::load_data() {
    if (ratio < 1) {
        gauge->SetValue(ratio * 100);
        SetStatusText(wxT("相机正在载入数据!"));
    } else {
        gauge->SetValue(100);
        SetStatusText(_T("相机载入图片数据完成"));
    }
}

void MainFrame::OnSend(wxCommandEvent &event) {
    dij.set_work_mode(camera_running);
    dij.set_image_mode(atoi(t_image_mode->GetValue()));
    dij.set_window(atoi(t_start_i->GetValue()), atoi(t_start_j->GetValue()),
                   atoi(t_ni->GetValue()), atoi(t_nj->GetValue()));
    dij.set_expo(atoi(t_expo->GetValue()));

    auto th = std::thread(dij_helper, this);
    th.detach();

}

