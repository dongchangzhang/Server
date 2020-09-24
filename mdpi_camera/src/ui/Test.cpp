//
// Created by vhdsih on 20-1-20.
//

#include "Test.h"
#include "../value/const_values.h"
#include "../value/values.h"
#include "Home.h"
#include "../net/Client.h"

BEGIN_EVENT_TABLE(Test, wxFrame)
    EVT_CLOSE(Test::on_close)
    EVT_BUTTON(ID_BTN_WIN, Test::on_btn_win_args)
    EVT_BUTTON(ID_BTN_RECV, Test::on_btn_send_image)
    EVT_RADIOBOX(ID_RBTN_IMODE, Test::on_btn_image_mode)
    EVT_RADIOBOX(ID_RBTN_WMODE, Test::on_btn_camera_state)
//    EVT_THREAD(ID_SENDING_THREAD_UPDATE_TEST, Test::on_update_image)
END_EVENT_TABLE()

Test::Test(wxFrame *_handler) : handler(_handler), wxFrame(NULL, wxID_ANY, wxT("中分辨相机测试工具"), wxDefaultPosition) {
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    // 相机模式
    wxArrayString camera_state_types;
    camera_state_types.Add(wxT("待机"));
    camera_state_types.Add(wxT("拍照"));
    radio_camera_state = new wxRadioBox(this, ID_RBTN_WMODE, wxT("&相机状态"), wxDefaultPosition, wxDefaultSize, camera_state_types, 1, wxRA_SPECIFY_ROWS);
    radio_camera_state->SetSelection(0);


    // 图像模式
    wxArrayString imode_types;
    imode_types.Add(wxT("&原图"));
    imode_types.Add(wxT("&1:2"));
    imode_types.Add(wxT("&1:4"));
    imode_types.Add(wxT("&1:8"));
    imode_types.Add(wxT("&开窗"));
    radio_image_mode = new wxRadioBox(this, ID_RBTN_IMODE, wxT("&图像模式"), wxDefaultPosition, wxDefaultSize, imode_types, 1, wxRA_SPECIFY_ROWS);
    radio_image_mode->SetSelection(0);

    // 开窗
    wxStaticBox* staticBox = new wxStaticBox(this,wxID_ANY, wxT("开窗参数"));
    wxStaticBoxSizer* staticSizer = new wxStaticBoxSizer(staticBox,wxVERTICAL);
    wxGridSizer* gridSizer = new wxGridSizer(4, 2, 0, 0);

    text_i = new wxTextCtrl(this, 0, wxEmptyString, wxDefaultPosition);
    text_j = new wxTextCtrl(this, 0, wxEmptyString, wxDefaultPosition);
    text_ni = new wxTextCtrl(this, 0, wxEmptyString, wxDefaultPosition);
    text_nj = new wxTextCtrl(this, 0, wxEmptyString, wxDefaultPosition);
    btn_win = new wxButton(this, ID_BTN_WIN, wxT("更新开窗参数"));

    text_i->SetValue("0");
    text_j->SetValue("0");
    text_ni->SetValue("0");
    text_nj->SetValue("0");

    wxStaticText *tt_i = new wxStaticText(this, wxID_STATIC, wxT("起始行"));
    wxStaticText *tt_j = new wxStaticText(this, wxID_STATIC, wxT("起始列"));
    wxStaticText *tt_ni = new wxStaticText(this, wxID_STATIC, wxT("行数"));
    wxStaticText *tt_nj = new wxStaticText(this, wxID_STATIC, wxT("列数"));

    // 发送图像
    wxStaticBox* staticBox2 = new wxStaticBox(this,wxID_ANY, wxT("发送图像指令"));
    wxStaticBoxSizer* staticSizer2 = new wxStaticBoxSizer(staticBox2,wxVERTICAL);
    btn_recv = new wxButton(this, ID_BTN_RECV, wxT("发送图像"));

//    // image recv
//    wxStaticBox* staticBox3 = new wxStaticBox(this,wxID_ANY, wxT("发送图像"));
//    wxStaticBoxSizer* staticSizer3 = new wxStaticBoxSizer(staticBox3,wxVERTICAL);
//    recved_image = new wxImagePanel(this, wxImage(1000, 1000), wxBITMAP_TYPE_JPEG);

    topSizer->Add(radio_camera_state, 0, wxEXPAND |wxALL, 5);
    topSizer->Add(radio_image_mode, 0, wxEXPAND | wxALL, 5);
    topSizer->Add(staticSizer, 0, wxEXPAND | wxALL, 5);
    topSizer->Add(staticSizer2, 0, wxEXPAND | wxALL, 5);
//    topSizer->Add(staticSizer3, 1, wxEXPAND | wxALL, 5);

    gridSizer->Add(tt_i, 0, wxALL, 5);
    gridSizer->Add(text_i, 0, wxALL, 5);
    gridSizer->Add(tt_j, 0, wxALL, 5);
    gridSizer->Add(text_j, 0, wxALL, 5);
    gridSizer->Add(tt_ni, 0, wxALL, 5);
    gridSizer->Add(text_ni, 0, wxALL, 5);
    gridSizer->Add(tt_nj, 0, wxALL, 5);
    gridSizer->Add(text_nj, 0, wxALL, 5);
    gridSizer->Add(btn_win, 0, wxALL, 5);

    staticSizer->Add(gridSizer, 0, wxALL, 5);

    staticSizer2->Add(btn_recv, 0, wxALL, 5);
//    staticSizer3->Add(recved_image, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}

void Test::on_close(wxCloseEvent &event) {
    ((Home*)handler)->set_test_window_state(false);
    Destroy();
}

void Test::on_btn_camera_state(wxCommandEvent &event) {
    char buffer[4];
    auto selected = radio_camera_state->GetSelection();
    Client client("127.0.0.1", get_port_of_camera_params());

    buffer[0] = 0x04;
    buffer[1] = 0x01;
    buffer[2] = 0x11;

    if (selected == 0) {
        buffer[3] = 0x11;
    } else {
        buffer[3] = 0x22;
    }
    client.send(buffer, 4);
}

void Test::on_btn_image_mode(wxCommandEvent &event) {
    char buffer[4];
    auto selected = radio_image_mode->GetSelection();
    Client client("127.0.0.1", get_port_of_camera_params());

    buffer[0] = 0x04; //
    buffer[1] = 0x01; // len
    buffer[2] = 0x33; // cmd type: image_mode

    switch (selected) {
        case 0:
            buffer[3] = 0x00;
            break;
        case 1:
            buffer[3] = 0x11;
            break;
        case 2:
            buffer[3] = 0x22;
            break;
        case 3:
            buffer[3] = 0x33;
            break;
        case 4:
            buffer[3] = 0x44;
            break;
        default:
            buffer[3] = 0x00;
            break;
    }
    client.send(buffer, 4);
}

void Test::on_btn_win_args(wxCommandEvent &event) {
    Client client("127.0.0.1", get_port_of_camera_params());
    short i = 0, j = 0, ni = 0, nj = 0;
    i = std::atoi(text_i->GetValue());
    j = std::atoi(text_j->GetValue());
    ni = std::atoi(text_ni->GetValue());
    nj = std::atoi(text_nj->GetValue());

    char buffer[20];

    buffer[0] = 0x04; //
    buffer[1] = 0x09; // len
    buffer[2] = 0x44; // cmd type: image_mode

    short_2_bytes(i, reinterpret_cast<unsigned char *>(&buffer[3]));
    short_2_bytes(j, reinterpret_cast<unsigned char *>(&buffer[5]));
    short_2_bytes(ni, reinterpret_cast<unsigned char *>(&buffer[7]));
    short_2_bytes(nj, reinterpret_cast<unsigned char *>(&buffer[9]));

    client.send(buffer, 12);
}

void Test::on_btn_send_image(wxCommandEvent &event) {
    Client client("127.0.0.1", get_port_of_camera_params());
    char buffer[20];

    buffer[0] = 0x04; //
    buffer[1] = 0x01; // len
    buffer[2] = 0xAA; // cmd type
    buffer[3] = 0x00; // cmd type
    client.send(buffer, 4);
}

//void Test::on_update_image(wxThreadEvent &event) {
//    recved_image->update(image);
//}
