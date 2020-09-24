//
// Created by vhdsih on 2019-06-15.
//

#include <ctype.h>
#include <opencv2/opencv.hpp>
#include <fstream>

#include "MainFrame.h"
#include "../conf/constants.h"
#include "Constructer.h"
#include "Image.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(Btn_Begin, MainFrame::Begin)
                EVT_BUTTON(Btn_End, MainFrame::End)
                EVT_BUTTON(Btn_Save, MainFrame::Save)
                EVT_BUTTON(Btn_Lead, MainFrame::Lead)
                EVT_BUTTON(Btn_GetImageDir, MainFrame::GetImageDir)
                EVT_BUTTON(Btn_GetOutputDir, MainFrame::GetOutputDir)
                EVT_THREAD(kThreadUpdateId, MainFrame::ThreadUpdate)
                EVT_CLOSE(MainFrame::Close)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title,
        const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size) {

    wxInitAllImageHandlers();

    canvas = ImageData(CANVAS_W, CANVAS_H);

    init_toolbar();
    init_statusbar();

    auto *topSizer = new wxBoxSizer(wxVERTICAL);
    mars_image = new wxImagePanel(this, wxImage(CANVAS_SHOW_W, CANVAS_SHOW_H), wxBITMAP_TYPE_JPEG, 2, 1);
    mars_image->SetBackgroundColour(*wxBLACK);
    topSizer->Add(mars_image, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(topSizer);
    Centre();
}

void MainFrame::End(wxCommandEvent &event) {
    if (!is_running) {
        return;
    }
    is_running = false;
    is_finished = true;
    th->stop();
}

void MainFrame::Begin(wxCommandEvent &event) {
    printf("begin\n");
    if (!is_lead) {
        wxMessageBox(wxT("图像未数据导入，必须点击'导入'导入待拼接的图像！"));
        return;
    }
    if (is_running) {
        wxMessageBox(wxT("程序正在运行，无需重复点击！"));
        return;
    }
    is_running = true;
    is_finished = false;
    th = new Constructer(this, index, canvas);
    th->start();
}

void MainFrame::Save(wxCommandEvent &event) {
    if (!is_finished) {
        wxMessageBox(wxT("必须等待拼接结束后才能保存结果图像！"));
        return;
    }
    wxString defaultPath = wxT("/home/z");
    wxFileDialog
            saveFileDialog(this, _T("保存文件"), "", "",
                           "files types (*.jpeg)|*.jpeg", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_OK) {
        auto res_path = saveFileDialog.GetPath();
        status_bar->SetStatusText(wxT("开始保存图像，请稍后..."), 0);
        if(canvas.to_wxImage().SaveFile(res_path)) {
            wxMessageBox(wxT("保存成功！"));
            status_bar->SetStatusText(wxT("保存成功！"), 0);
        } else {
            wxMessageBox(wxT("保存失败！"));
        }
    } else {
        wxMessageBox(wxT("目录不合法！"));
    }
}

void MainFrame::Lead(wxCommandEvent &event) {
    if (is_running) {
        wxMessageBox(wxT("程序正在运行，此时不能导入数据！"));
        return;
    }

    wxString defaultPath = wxT("/home/z");
    wxDirDialog dialog((wxWindow *) (this), wxT("选择图片资源目录"), defaultPath, wxDD_NEW_DIR_BUTTON);
    if (dialog.ShowModal() == wxID_OK) {
        images_path = dialog.GetPath();
        is_lead = true;
        SetStatusText(wxT("图像资源已选择，等待启动！"));
        path->SetLabel(images_path);
    } else {
        wxMessageBox(wxT("导入数据失败！"));
    }
}

void MainFrame::Close(wxCloseEvent &event) {
    wxMessageDialog dialog(nullptr, wxT("是否退出程序"), wxT("提醒"), wxNO_DEFAULT | wxNO | wxYES | wxICON_INFORMATION);
    switch (dialog.ShowModal()) {
        case wxID_YES:
            Destroy();
            break;
        default:
            break;
    }
}

void MainFrame::ThreadUpdate(wxThreadEvent &event) {
    ratio = ratio * 100 >= 100 ? 100 : ratio * 100;
    gauge->SetValue(ratio);
    mars_image->update_v2(canvas.to_wxImage());
    SetStatusText(wxString::Format(wxT("[拼图进度]: %d / %d"), now, all));
    if (now >= all) {
        is_finished = true;
        SetStatusText(wxString::Format(wxT("[拼图进度]: 100\% 完成!")));
    }
}

void MainFrame::init_toolbar() {
    tool_bar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

    btn_begin = new wxButton(tool_bar, Btn_Begin, wxT("&启动"), wxDefaultPosition, wxDefaultSize);
    btn_begin->SetBackgroundColour(wxColour(250, 226, 112));

    btn_end = new wxButton(tool_bar, Btn_End, wxT("终止"));

    btn_save = new wxButton(tool_bar, Btn_Save, wxT("保存"));
    btn_lead = new wxButton(tool_bar, Btn_Lead, wxT("导入"));

    path = new wxStaticText(tool_bar, wxID_STATIC, wxT("DataPath: None!"));

    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_lead);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_save);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_end);
    tool_bar->AddSeparator();
    tool_bar->AddControl(btn_begin);
    tool_bar->AddSeparator();
    tool_bar->AddControl(path);
    tool_bar->AddSeparator();
    tool_bar->Realize();
    tool_bar->Fit();
    this->SetToolBar(tool_bar);
};

void MainFrame::init_statusbar() {
    status_bar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
    status_bar->SetMinHeight(40);
    this->SetStatusBar(status_bar);
    int widths[] = {400, -1};
    status_bar->SetFieldsCount(WXSIZEOF(widths), widths);
    status_bar->SetStatusText(wxT("请导入正确的图像!"), 0);
    wxRect rc;
    status_bar->GetFieldRect(1, rc);
    gauge = new wxGauge(status_bar, wxID_ANY, 100, wxPoint(rc.x, rc.y), wxSize(rc.width, rc.height), wxGA_HORIZONTAL);
}

