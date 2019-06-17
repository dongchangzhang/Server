//
// Created by z on 19-4-27.
//

#include "MyImgPanel.h"
#include <iostream>
#include <mutex>

std::mutex mtx;

BEGIN_EVENT_TABLE(MyImgPanel, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */

// catch paint events
                EVT_PAINT(MyImgPanel::paintEvent)
//Size event
                EVT_SIZE(MyImgPanel::OnSize)
END_EVENT_TABLE()


// some useful events
/*
 void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseDown(wxMouseEvent& event) {}
 void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
 void wxImagePanel::rightClick(wxMouseEvent& event) {}
 void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void wxImagePanel::keyPressed(wxKeyEvent& event) {}
 void wxImagePanel::keyReleased(wxKeyEvent& event) {}
 */

MyImgPanel::MyImgPanel(wxFrame *parent) : wxPanel(parent) {
    // load the file... ideally add a check to see if loading was successful
    cv::Mat tmp(3072, 4096, CV_8UC3, cv::Scalar::all(0));
    image = wx_from_mat(tmp);
//    image.LoadFile(file, format);
    w = -1;
    h = -1;
    c1 = 4.0 / 3.0;
    c2 = 3.0 / 4.0;
}
MyImgPanel::MyImgPanel(wxFrame *parent, int c) : wxPanel(parent) {
    // load the file... ideally add a check to see if loading was successful
    image_route = cv::Mat(2000, 2600, CV_8UC3, cv::Scalar::all(0));
    init_mars();
    image = wx_from_mat(image_route);
//    image.LoadFile(file, format);
    w = -1;
    h = -1;
    c1 = double(image_route.cols) / double(image_route.rows);
    c2 = double(image_route.rows) / double(image_route.cols);
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void MyImgPanel::paintEvent(wxPaintEvent &evt) {
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void MyImgPanel::paintNow() {
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void MyImgPanel::render(wxDC &dc) {
    mtx.lock();
    int neww, newh;
    dc.GetSize(&neww, &newh);
    if (neww != w || newh != h) {
        if (neww >= newh * c1) {
            neww = newh * c1;
        } else {
            newh = neww * c2;
        }

        resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/ ));
        w = neww;
        h = newh;
        dc.DrawBitmap(resized, 0, 0, false);
    } else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
    mtx.unlock();
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void MyImgPanel::OnSize(wxSizeEvent &event) {
    Refresh();
    //skip the event.
    event.Skip();
}

void MyImgPanel::update(cv::Mat &CVImg) {
    c1 = double(CVImg.cols) / double(CVImg.rows);
    c2 = double(CVImg.rows) / double(CVImg.cols);
    image = wx_from_mat(CVImg);
    Refresh();
}

void MyImgPanel::update(wxString file, wxBitmapType format) {
    mtx.lock();
    image.LoadFile(file, format);
    mtx.unlock();
    Refresh();
}


wxImage MyImgPanel::wx_from_mat(cv::Mat &img) {
    int code1 = 0, code4 = 0, code = 0;
    cv::Mat im2;
#ifdef CV_GRAY2RGB
    code1 = CV_GRAY2RGB;
    code4 = CV_BGRA2RGB;
    code = CV_BGR2RGB;
#else
    code1 = cv::COLOR_GRAY2RGB;
    code4 = cv::COLOR_BGRA2RGB;
    code = cv::COLOR_BGR2RGB;
#endif
    if (img.channels() == 1) {
        cvtColor(img, im2, code1);
    } else if (img.channels() == 4) {
        cvtColor(img, im2, code4);
    } else {
        cvtColor(img, im2, code);
    }
    long imsize = im2.rows * im2.cols * im2.channels();
    wxImage wxImg(im2.cols, im2.rows, (unsigned char *) malloc(imsize), false);
    unsigned char *s = im2.data;
    unsigned char *d = wxImg.GetData();
    for (long i = 0; i < imsize; i++) {
        d[i] = s[i];
    }
    return wxImg;
}


void MyImgPanel::init_mars() {
    double angle = 0, dangle = 0.001;
    int j = 0, k = 0;
    double y = 0, z = 0;
    yz2jk(y, z, j, k);

    while (angle < 2 * M_PI) {
        angle += dangle;
        y =  -8004 * sin(angle);
        z =  5843.9 + 10473 * cos(angle);
        yz2jk(y, z, j, k);
        cv::circle(image_route, cv::Point(k, j), 2, cv::Scalar(255, 255, 255));
    }
    //center
    int oy, oz, yy, zz;
    yz2jk(0, 0, oy, oz);
    // radius
    yz2jk(0, 3396, yy, zz);
    cv::circle(image_route, cv::Point(oz, oy), zz - oz, cv::Scalar(123, 130, 161), -1, 8);
    y =  -8004 * sin(1);
    z =  5843.9 + 10473 * cos(1);
}

void MyImgPanel::draw(double y, double z) {
    int j, k;
    yz2jk(y / 1000, z / 1000, j, k);
    auto tmp = image_route.clone();
    cv::circle(tmp, cv::Point(k, j), 30, cv::Scalar(0, 0, 255), -1, 8);
    image = wx_from_mat(tmp);
    Refresh();
}
