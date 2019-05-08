//
// Created by z on 19-4-27.
//

#ifndef SERVERGUI_MYIMGPANEL_H
#define SERVERGUI_MYIMGPANEL_H


#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <opencv2/opencv.hpp>


class MyImgPanel : public wxPanel
{
    wxBitmap resized;
    int w, h;

public:
    wxImage image;

public:
    MyImgPanel(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    wxImage wx_from_mat(cv::Mat &img);


    void update(cv::Mat &CVImg);

    void update(wxString file, wxBitmapType format);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
private:
    DECLARE_EVENT_TABLE()
};

#endif
