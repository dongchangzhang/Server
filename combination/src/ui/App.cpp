//
// Created by vhdsih on 2019-06-15.
//

#include "App.h"

#include <wx/wx.h>

bool App::OnInit() {
    MainFrame *myFrame = new MainFrame(wxT("火星图片拼接工具"), wxDefaultPosition, wxSize(880, 540));
    myFrame->Show();
    return true;
}
