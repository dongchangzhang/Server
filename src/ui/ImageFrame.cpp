//
// Created by 张东昌 on 2019-06-22.
//

#include "ImageFrame.h"
#include "MainFrame.h"


ImageFrame::ImageFrame(const wxString& title, const wxPoint& pos, const wxSize& size, void *_ptr)
        : wxFrame(NULL, wxID_ANY, title, pos, size), ptr(_ptr) {
    img = new ImagePanel(this);
    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(img, 1, wxEXPAND | wxALL, 1);
    this->SetSizer(sizer);
}

ImageFrame::ImageFrame(const wxString& title, const wxPoint& pos, const wxSize& size, void *_ptr, int c)
        : wxFrame(NULL, wxID_ANY, title, pos, size), ptr(_ptr) {
    img = new ImagePanel(this, c);
    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(img, 1, wxEXPAND | wxALL, 1);
    this->SetSizer(sizer);
}
ImageFrame::~ImageFrame() {
    ((MainFrame*)ptr)->image_frame_destroyed = true;
    std::cout << ((MainFrame*)ptr)->image_frame_destroyed << std::endl;
}


