//
// Created by vhdsih on 20-1-18.
//

#include "App.h"
#include "Home.h"
#include "../value/const_values.h"

bool App::OnInit() {
    Home *home = new Home(wxT("火星中分辨率相机模拟终端"), wxDefaultPosition, wxSize(WIN_HEIGHT, WIN_WIDTH));
    home->Show();
    return true;
};
