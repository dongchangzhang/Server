//
// Created by z on 19-4-26.
//

#include "App.h"

bool App::OnInit() {
    wxInitAllImageHandlers();
    frame = new MainFrame(wxT("中分辨率相机测试平台!"), wxPoint(50, 50), wxSize(1250, 960));
    frame->Show(true);
    return true;
}

