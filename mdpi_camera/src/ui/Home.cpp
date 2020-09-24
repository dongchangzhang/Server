//
// Created by vhdsih on 20-1-18.
//

#include "Home.h"

#include "../utils/utils.h"
#include "../value/values.h"
#include "../thread/SendImage.h"

#include <cstdio>

#include <string>
#include <iostream>

#include <wx/progdlg.h>
#include <wx/tipdlg.h>
#include <fstream>


BEGIN_EVENT_TABLE(Home, wxFrame)
                EVT_CLOSE(Home::on_close)
                EVT_BUTTON(ID_BTN_CAM, Home::on_btn_args)
                EVT_BUTTON(ID_BTN_EXIT, Home::on_btn_close)
                EVT_BUTTON(ID_BTN_BEGIN, Home::on_btn_start)
                EVT_BUTTON(ID_BTN_TEST, Home::on_btn_test)
                EVT_BUTTON(ID_BTN_GNC_CON, Home::on_btn_gnc)
                EVT_BUTTON(ID_BTN_GNC_LOC, Home::on_btn_loc)
                EVT_RADIOBOX(ID_RBTN_SEE, Home::on_see_mode)
                EVT_RADIOBOX(ID_RBTN_SAVE, Home::on_switch_saving_mode)
                EVT_THREAD(ID_CTRL_THREAD_UPDATE, Home::on_update_ctrl)
                EVT_THREAD(ID_IMAGE_THREAD_UPDATE, Home::on_update_image)
                EVT_THREAD(ID_SENDING_THREAD_UPDATE, Home::on_update_sending)
                EVT_MENU(ID_MENU_INFO, Home::on_menu_info)
                EVT_MENU(ID_MENU_HELP, Home::on_menu_help)
                EVT_MENU(ID_MENU_SETTING_IP_THIS, Home::on_menu_setting_ip_this)
                EVT_MENU(ID_MENU_SETTING_IP_REMOTE, Home::on_menu_setting_ip_remote)
                EVT_MENU(ID_MENU_SETTING_PORT_GNC, Home::on_menu_setting_port_gnc)
                EVT_MENU(ID_MENU_SETTING_PORT_DIJ, Home::on_menu_setting_port_dij)
                EVT_MENU(ID_MENU_SETTING_PORT_IMAGE, Home::on_menu_setting_port_image)
END_EVENT_TABLE()

Home::Home(const wxString &title, const wxPoint &pos, const wxSize &size) :
        wxFrame(nullptr, wxID_ANY, title, pos, size) {
    init_mars();
    init_menu();
    init_toolbar();
    init_statusbar();
    wxInitAllImageHandlers();

    auto *topSizer = new wxBoxSizer(wxVERTICAL);
    mars_img = new wxImagePanel(this, wxImage(4096, 3072), wxBITMAP_TYPE_JPEG);
    mars_img->SetBackgroundColour(*wxBLACK);
    topSizer->Add(mars_img, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(topSizer);
    Centre();

    send_image = wxImage(4096, 3072);
    camera_ctrl_contents = get_camera_ctrl_params();

    on_update_statusbar();
}

void Home::init_mars() {
    int max = GeoTiffPixelWidth * GeoTiffPixelHeight, gap = 100000;
    int npixel = 0, w, h;
    unsigned char buffer[3];

    auto b = timestamp();
    std::ifstream file(DEFAULT_MARS_PIXELS_PATH);

    wxProgressDialog *dialog = new wxProgressDialog(wxT("正在载入，请稍后 ... "), wxT("请等待 ... "), 100, this,
                                                    wxPD_AUTO_HIDE | wxPD_APP_MODAL);
    Centre();
    // checking
    file.read((char *) &w, 4);
    file.read((char *) &h, 4);
    printf("Loading mars data, checking passed? -> %d\n", (w == GeoTiffPixelWidth && h == GeoTiffPixelHeight));

    file.read((char *) buffer, 3);
    auto &mars_pixels = get_mars_pixels_data();
    while (file.gcount() == 3) {
        mars_pixels[npixel] = Pixel(buffer);
        npixel += 1;
        if (npixel % gap == 0) {
            dialog->Update(int(npixel / float(max) * 100),
                           wxString::Format(wxT("数据载入进度::%3i/%3i"), int(npixel / float(max) * 100), 100), NULL);
        }
        file.read((char *) buffer, 3);
    }

    file.close();
    auto e = timestamp();
    printf("Loading mars pixels using %lf ms\n", timediff(b, e));
    delete dialog;
}


void Home::init_menu() {
    menu = new wxMenuBar;

    base = new wxMenu;
    base->Append(wxID_EXIT, _T("退出"), _T("退出程序"));

    setting = new wxMenu;
    setting->Append(ID_MENU_SETTING_IP_THIS, _T("本机IP"), _T("设置a"));
    setting->Append(ID_MENU_SETTING_IP_REMOTE, _T("上位机IP"), _T("设置b"));
    setting->Append(ID_MENU_SETTING_PORT_GNC, _T("GNC接收端口"), _T("设置c"));
    setting->Append(ID_MENU_SETTING_PORT_DIJ, _T("相机控制接收端口"), _T("设置d"));
    setting->Append(ID_MENU_SETTING_PORT_IMAGE, _T("图像发送端口"), _T("设置e"));

    help = new wxMenu;
    help->Append(ID_MENU_INFO, _T("关于"), _T("关于"));

    menu->Append(base, _T("&功能"));
    menu->Append(setting, _T("&设置"));
    menu->Append(help, _T("&帮助"));

    SetMenuBar(menu);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCloseEventHandler(Home::on_close));
    Centre();
}

void Home::init_toolbar() {
    tool_bar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

    btn_gnc = new wxButton(tool_bar, ID_BTN_GNC_CON, wxT("&GNC内容"));
    btn_loc = new wxButton(tool_bar, ID_BTN_GNC_LOC, wxT("&相机位置"));
    btn_args = new wxButton(tool_bar, ID_BTN_CAM, wxT("&相机参数"));
    btn_exit = new wxButton(tool_bar, ID_BTN_BEGIN, wxT("&启动"), wxDefaultPosition, wxSize(200, -1));
    btn_check = new wxButton(tool_bar, ID_BTN_TEST, wxT("&自测"));

    btn_exit->SetBackgroundColour(wxColour(250, 226, 112));
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_exit);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_check);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_args);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_gnc);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_loc);
    tool_bar->AddSeparator();

    wxArrayString strings;
    strings.Add(wxT("&保存到本地"));
    strings.Add(wxT("&仅实时浏览"));
    saving_mode = new wxRadioBox(tool_bar, ID_RBTN_SAVE, wxT("&实时图像"), wxDefaultPosition, wxDefaultSize, strings, 1,
                                 wxRA_SPECIFY_ROWS);
    saving_mode->SetSelection(1);

    tool_bar->AddControl(saving_mode);
    tool_bar->AddSeparator();

    wxArrayString strings2;
    strings2.Add(wxT("&打开"));
    strings2.Add(wxT("&关闭"));
    always_see = new wxRadioBox(tool_bar, ID_RBTN_SEE, wxT("&持续浏览"), wxDefaultPosition, wxDefaultSize, strings2, 1,
                                wxRA_SPECIFY_ROWS);
    always_see->SetSelection(0);
    tool_bar->AddControl(always_see);

    tool_bar->Realize();
    this->SetToolBar(tool_bar);
};

void Home::init_statusbar() {
    status_bar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
    status_bar->SetMinHeight(40);
    this->SetStatusBar(status_bar);
    int widths[] = {300, 200, -1};
    status_bar->SetFieldsCount(WXSIZEOF(widths), widths);
    status_bar->SetStatusText(wxT("等待启动 ..."), 0);
    wxRect rc;
    status_bar->GetFieldRect(1, rc);
    send_gauge = new wxGauge(status_bar, wxID_ANY, 100, wxPoint(rc.x, rc.y), wxSize(rc.width, rc.height),
                             wxGA_HORIZONTAL);
    send_gauge->Show(false);
}

void Home::on_btn_start(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，请勿重复点击启动按钮!"));
        return;
    }
    wxMessageDialog dialog(nullptr, wxT("启动之前请确认对应IP和端口号正确设置，"
                                        "这些内容在运行过程中将无法修改!点击顶部[相机参数]按钮查看默认参数，到菜单[设置]中修改参数。"), wxT("是否启动？"),
                           wxYES_DEFAULT | wxNO | wxYES | wxICON_INFORMATION);
    switch (dialog.ShowModal()) {
        case wxID_YES:
            camera_is_start = true;
            status_bar->SetStatusText(wxT("相机已经启动！"), 0);
            init_thread();
            break;
        default:
            break;
    }
}

void Home::on_close(wxCloseEvent &event) {
    wxMessageDialog dialog(nullptr, wxT("是否退出程序"), wxT("提醒"), wxNO_DEFAULT | wxNO | wxYES | wxICON_INFORMATION);
    switch (dialog.ShowModal()) {
        case wxID_YES:
            clean();
            Destroy();
            break;
        default:
            break;
    }
}

void Home::on_btn_close(wxCommandEvent &event) {
    wxMessageDialog dialog(nullptr, wxT("是否退出程序"), wxT("提醒"), wxNO_DEFAULT | wxNO | wxYES | wxICON_INFORMATION);
    switch (dialog.ShowModal()) {
        case wxID_YES:
            clean();
            Destroy();
            break;
        default:
            break;
    }
}

void Home::on_btn_test(wxCommandEvent &event) {
    if (!camera_is_start) {
        wxMessageBox(wxT("需要相机启动后才能使用该功能!"));
        return;
    }
    if (!get_test_window_state()) {
        set_test_window_state(true);
        test_window = new Test(this);
        test_window->Show(true);
    } else {
        wxMessageBox(wxT("the window had been opened!"));
    }
}

void Home::on_btn_gnc(wxCommandEvent &event) {
    if (!get_gnc_arg_window_state()) {
        set_gnc_arg_window_state(true);
        gnc_args_window = new GncArgs(this);
        gnc_args_window->Show(true);
    } else {
        wxMessageBox(wxT("the window had been opened!"));
    }

}

void Home::on_btn_loc(wxCommandEvent &event) {
    if (!get_gnc_loc_window_state()) {
        set_gnc_loc_window_state(true);
        gnc_loc_window = new GncLoc(this);
        gnc_loc_window->Show(true);
    } else {
        wxMessageBox(wxT("the window had been opened!"));
    }

}

void Home::on_btn_args(wxCommandEvent &event) {
    if (!get_cam_arg_window_state()) {
        set_cam_arg_window_state(true);
        camera_args_window = new CamArgs(this);
        camera_args_window->Show(true);
    } else {
        camera_args_window = nullptr;
        wxMessageBox(wxT("the window had been opened!"));
    }
}

void Home::on_see_mode(wxCommandEvent &event) {
    if (always_see->GetSelection() == 0) {
        mars_img->set_update_mode(true);
    } else {
        mars_img->set_update_mode(false);
    }

}

void Home::on_switch_saving_mode(wxCommandEvent &event) {
    DataInjection dij;
    while (!read_camera_params(dij)) {
        wxMicroSleep(10);
    }
    if (saving_mode->GetSelection() == 0 && !dij.get_saving_mode()) {
        wxString defaultPath = wxT("/home/z");
        wxDirDialog dialog((wxWindow *) (this), wxT("选择图片保存目录"), defaultPath, wxDD_NEW_DIR_BUTTON);
        if (dialog.ShowModal() == wxID_OK) {
            wxString path = dialog.GetPath();
            std::cout << path << std::endl;
            dij.set_saving_info(true, std::string(path));
        } else {
            saving_mode->SetSelection(1);
            dij.set_saving_info(false);
        }
    } else if (saving_mode->GetSelection() == 1) {
        dij.set_saving_info(false);
    }
    while (!update_camera_params(dij, UPDATE_SAVING_INFO)) wxMicroSleep(10);
}

void Home::init_thread() {
    camera = new Camera((wxWindow *) this);
    camera->start_thread();

    auto port_of_gnc = get_port_of_gnc();
    gnc_receiver = new GncRecvByNet((wxWindow *) this, port_of_gnc);
    gnc_receiver->start_thread();

    gnc_rs422_receiver = new GncRecvByRs422((wxWindow *) this);
    gnc_rs422_receiver->start_thread();

    gnc_parser = new GncParser((wxWindow *) this);
    gnc_parser->start_thread();

    auto port_of_camera_params = get_port_of_camera_params();
    ctrl_receiver = new CamCtrlByNet((wxWindow *) this, port_of_camera_params);
    ctrl_receiver->start_thread();

    ctrl_rs422_receiver = new CamCtrlByRs422((wxWindow *) this);
    ctrl_rs422_receiver->start_thread();

    ctrl_parser = new CamCtrlParser((wxWindow *) this);
    ctrl_parser->start_thread();

    auto ip_of_remote_pc = get_ip_of_remote_pc();
    auto port_of_image = get_port_of_image_sending();
    sender = new SendImage((wxWindow *) this, ip_of_remote_pc, port_of_image);
    sender->start_thread();
}

void Home::clean() {
    // destroy thread
    printf("thread destroy\n");
    camera->stop_thread();
    gnc_receiver->stop_thread();
    ctrl_receiver->stop_thread();

    auto device_is_open_of_gnc_rs422 = get_gnc_rs422_device_open_state();
    if (device_is_open_of_gnc_rs422) {
        gnc_rs422_receiver->stop_thread();
    }
    auto device_is_open_of_ctrl_rs422 = get_commands_rs422_device_open_state();
    if (device_is_open_of_ctrl_rs422) {
        ctrl_rs422_receiver->stop_thread();
    }
    if (gnc_parser) {
        gnc_parser->stop_thread();
    }
    if (ctrl_parser) {
        ctrl_parser->stop_thread();
    }

    // destroy windows
    printf("window destroy\n");
    if (get_cam_arg_window_state()) {
        camera_args_window->Destroy();
    }
    if (get_test_window_state()) {
        test_window->Destroy();
    }
    if (get_gnc_arg_window_state()) {
        gnc_args_window->Destroy();
    }
    if (get_gnc_loc_window_state()) {
        gnc_loc_window->Destroy();
    }
    printf("global destroy\n");
    clean_global_values();
}

void Home::on_update_image(wxThreadEvent &event) {
    if (images.size() > 0) {
        wxImage tmp = images.front().to_wxImage();
        images.pop();
        mars_img->update(tmp);
    }
    event.Skip();
}

wxString Home::get_work_mode_text() {
    wxString tmp;
    if (camera_ctrl_contents.get_work_mode() == 0x11) {
        tmp = _T("待机");
    } else if (camera_ctrl_contents.get_work_mode() == 0x22) {
        tmp = _T("拍照");
    } else {
        tmp = _T("错误");
    }
    return tmp;
}

wxString Home::get_image_mode_text() {
    wxString tmp;
    switch (camera_ctrl_contents.get_image_mode()) {
        case (0x00):
            tmp = _T("原图");
            break;
        case (0x11):
            tmp = _T("1:2");
            break;
        case (0x22):
            tmp = _T("1:4");
            break;
        case (0x33):
            tmp = _T("1:8");
            break;
        case (0x44):
            tmp = _T("开窗");
            break;
        default:
            tmp = _T("错误");
            break;
    }
    return tmp;
}

void Home::on_update_ctrl(wxThreadEvent &event) {
    if (get_cam_arg_window_state()) {
        camera_args_window->work_mode->SetLabelText(get_work_mode_text());
        camera_args_window->image_mode->SetLabelText(get_image_mode_text());
        short i = 0, j = 0, ni = 0, nj = 0;
        camera_ctrl_contents.get_windows_params(i, j, ni, nj);
        wxString win_buffer = wxString::Format(wxT("(%d, %d, %d, %d)"), i, j, ni, nj);
        camera_args_window->win_args->SetLabelText(win_buffer);
    }
    on_update_statusbar();
}

void Home::on_update_sending(wxThreadEvent &event) {
    int _pos, n_try = 0;
    while (!read_pos(_pos)) {
        ++n_try;
        wxMicroSleep(1);
        if (n_try > 10) return;
    }
    if (_pos < 0) return;
    status_bar->SetStatusText(wxT("发送图像中..."), 0);
    send_gauge->Show(true);
    _pos = _pos < 0 ? 0 : _pos > 100 ? 100 : pos;
    send_gauge->SetValue(_pos);
    on_update_statusbar();
    if (_pos == 100) {
        status_bar->SetStatusText(wxT("发送完成!"), 0);
        send_gauge->SetValue(0);
        send_gauge->Hide();
        while (!set_pos(-1)) wxMicroSleep(1);
    }
}

void Home::on_update_statusbar() {
    short i, j, ni, nj;
    camera_ctrl_contents.get_windows_params(i, j, ni, nj);
    wxString tmp = wxString::Format(wxT("相机：%s  |  图像: %s  |  开窗： [%d, %d, %d, %d]"),
                                    get_work_mode_text(), get_image_mode_text(), i, j, ni, nj);
    status_bar->SetStatusText(tmp, 2);
}

void Home::clean_global_values() {
    try {
        finalize();
    } catch (...) {
        std::cerr << "Catch(...)" << std::endl;
    }
}

void Home::on_menu_info(wxCommandEvent &event) {
    wxMessageDialog dialog(nullptr, wxT("火星中分辨率相机模拟器!"),
                           wxT("关于"),
                           wxCANCEL | wxICON_INFORMATION);
    switch (dialog.ShowModal()) {
        case wxID_CANCEL:
            break;
        default:
            break;
    }
}

void Home::on_menu_help(wxCommandEvent &event) {
    // wxTipProvider *tipProvider = wxCreateFileTipProvider(wxT("Makefile"), 0);
    // auto m_showAtStartup = wxShowTip(this, tipProvider, true);
    // delete tipProvider;
}

void Home::on_menu_setting_ip_this(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，此时不能进行设置。若需要设置，请关闭软件，重启后设置!"));
        return;
    }
    wxTextEntryDialog dialog(this,
                             wxT("请输入合法IP地址,系统不会检查其合法性。本机默认IP为：192.168.0.5"),
                             wxT("设置本机IP\n"),
                             wxT("192.168.0.5"),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK) {
        wxMessageBox(dialog.GetValue(), wxT("本机IP设置为:"));
        get_ip_of_this_pc() = dialog.GetValue();
    }

}

void Home::on_menu_setting_ip_remote(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，此时不能进行设置。若需要设置，请关闭软件，重启后设置!"));
        return;
    }
    wxTextEntryDialog dialog(this,
                             wxT("请输入合法IP地址,系统不会检查其合法性。上位机默认IP为：192.168.0.6"),
                             wxT("设置上位机IP\n"),
                             wxT("192.168.0.6"),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK) {
        wxMessageBox(dialog.GetValue(), wxT("上位机IP设置为:"));
        get_ip_of_remote_pc() = dialog.GetValue();
    }

}

void Home::on_menu_setting_port_gnc(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，此时不能进行设置。若需要设置，请关闭软件，重启后设置!"));
        return;
    }
    wxTextEntryDialog dialog(this,
                             wxT("请输入合法端口,系统不会检查其合法性，默认端口号：5003。"),
                             wxT("设置GNC接收端口\n"),
                             wxT("5003"),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK) {
        wxMessageBox(dialog.GetValue(), wxT("GNC端口设置为："));
        get_port_of_gnc() = std::atoi(dialog.GetValue());
    }
}

void Home::on_menu_setting_port_dij(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，此时不能进行设置。若需要设置，请关闭软件，重启后设置!"));
        return;
    }
    wxTextEntryDialog dialog(this,
                             wxT("请输入合法端口，系统不会检查其合法性。默认端口号：5002。"),
                             wxT("设置相机控制指令接收端口\n"),
                             wxT("5002"),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK) {
        wxMessageBox(dialog.GetValue(), wxT("相机控制端口设置为："));
        get_port_of_camera_params() = std::atoi(dialog.GetValue());
    }

}

void Home::on_menu_setting_port_image(wxCommandEvent &event) {
    if (camera_is_start) {
        wxMessageBox(wxT("相机已经启动，此时不能进行设置。若需要设置，请关闭软件，重启后设置!"));
        return;
    }
    wxTextEntryDialog dialog(this,
                             wxT("请输入合法端口，系统不会检查其合法性。默认端口号：6000。"),
                             wxT("设置图像发送端口\n"),
                             wxT("6000"),
                             wxOK | wxCANCEL);
    if (dialog.ShowModal() == wxID_OK) {
        wxMessageBox(dialog.GetValue(), wxT("图像发送端口设置为："));
        get_port_of_image_sending() = std::atoi(dialog.GetValue());
    }
}
