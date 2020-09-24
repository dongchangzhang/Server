//
// Created by vhdsih on 20-1-20.
//

#include "CamArgs.h"
#include "../value/const_values.h"
#include "../value/values.h"
#include "Home.h"

BEGIN_EVENT_TABLE(CamArgs, wxFrame)
                EVT_CLOSE(CamArgs::on_close)
END_EVENT_TABLE()

CamArgs::CamArgs(wxFrame *_handler)
        : handler(_handler), wxFrame(NULL, wxID_ANY, wxT("相机参数")) {

    auto *topSizer = new wxBoxSizer(wxVERTICAL);
    // 工作模式
    auto *staticBox = new wxStaticBox(this,wxID_ANY, wxT("工作模式"));
    auto *staticSizer = new wxStaticBoxSizer(staticBox,wxVERTICAL);
    work_mode = new wxStaticText(this, wxID_STATIC, wxT("0x22"));

    // 图像模式
    auto *staticBox2 = new wxStaticBox(this,wxID_ANY, wxT("图像模式"));
    auto *staticSizer2 = new wxStaticBoxSizer(staticBox2, wxVERTICAL);
    image_mode = new wxStaticText(this, wxID_STATIC, wxT("0x11"));

    // 开窗参数
    auto *staticBox3 = new wxStaticBox(this,wxID_ANY, wxT("开窗参数"));
    auto *staticSizer3 = new wxStaticBoxSizer(staticBox3, wxVERTICAL);
    win_args = new wxStaticText(this, wxID_STATIC, wxT("1; 3; 4; 5"));

    staticSizer->Add(work_mode, 1, wxEXPAND | wxALL, 10);
    staticSizer2->Add(image_mode, 1, wxEXPAND | wxALL, 10);
    staticSizer3->Add(win_args, 1, wxEXPAND | wxALL, 10);
    topSizer->Add(staticSizer, 0, wxEXPAND | wxALL, 10);
    topSizer->Add(staticSizer2, 0, wxEXPAND | wxALL, 10);
    topSizer->Add(staticSizer3, 0, wxEXPAND | wxALL, 10);

    this->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    auto *event = new wxThreadEvent(wxEVT_THREAD, ID_CTRL_THREAD_UPDATE);
    wxQueueEvent(handler->GetEventHandler(), event);
}

void CamArgs::on_close(wxCloseEvent &event) {
    ((Home*)handler)->set_cam_arg_window_state(false);
    Destroy();
}
