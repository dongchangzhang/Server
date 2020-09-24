//
// Created by vhdsih on 20-1-18.
//

#ifndef MDPI_CAMERA_IMAGE_H
#define MDPI_CAMERA_IMAGE_H
#include <wx/wx.h>
#include <wx/sizer.h>
#include <opencv2/opencv.hpp>

class wxImagePanel : public wxPanel
{
    wxImage image;
    wxBitmap bitmap, resized;
    int w, h;
    int scale_w = 4, scale_h = 3;
    bool update_image_mode = true;
    int id = 0;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);
    wxImagePanel(wxFrame* parent, wxImage _image, wxBitmapType format, int _scale_w = 4, int scale_h = 3);
    wxImage wx_from_mat(cv::Mat &img);


    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    void update(wxImage _image);
    void update_v2(wxImage _image);

    inline void set_update_mode(bool state) {
        update_image_mode = state;
    }
    inline void set_scale_params(int _w, int _h) {
        scale_w = _w;
        scale_h = _h;
    }

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

DECLARE_EVENT_TABLE()
};
#endif //MDPI_CAMERA_IMAGE_H
