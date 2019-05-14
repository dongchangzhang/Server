//
// Created by z on 19-4-26.
//

#include "MyApp.h"

bool MyApp::OnInit() {
    wxInitAllImageHandlers();
    frame = new MyFrame(wxT("Mars Server!"), wxPoint(50, 50), wxSize(1920, 1080));
    frame->Show(true);
    return true;
}

