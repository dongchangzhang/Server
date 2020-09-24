//
// Created by z on 19-4-27.
//

#include "ImagePanel.h"
#include "../conf/constants.h"
#include <iostream>
#include <mutex>

std::mutex mtx;

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::paintEvent)
    EVT_SIZE(ImagePanel::OnSize)
END_EVENT_TABLE()


ImagePanel::ImagePanel(wxFrame *parent) : wxPanel(parent) {
    // load the file... ideally add a check to see if loading was successful
    cv::Mat tmp(CANVAS_W, CANVAS_H, CV_8UC3, cv::Scalar::all(0));
    image = wx_from_mat(tmp);
    w = -1;
    h = -1;
    c1 = float(CANVAS_W) / float(CANVAS_H);
    c2 = float(CANVAS_H) / float(CANVAS_W);
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void ImagePanel::paintEvent(wxPaintEvent &evt) {
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
void ImagePanel::paintNow() {
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void ImagePanel::render(wxDC &dc) {
    mtx.lock();
    int neww, newh;
    dc.GetSize(&neww, &newh);
    if (neww != w || newh != h) {
        if (neww >= newh * c1) {
            neww = newh * c1;
        } else {
            newh = neww * c2;
        }

        resized = wxBitmap(image.Scale(neww, newh , wxIMAGE_QUALITY_HIGH));
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
void ImagePanel::OnSize(wxSizeEvent &event) {
    Refresh();
    //skip the event.
    event.Skip();
}

void ImagePanel::update(cv::Mat &CVImg) {

    c1 = double(CVImg.cols) / double(CVImg.rows);
    c2 = double(CVImg.rows) / double(CVImg.cols);
    image = wx_from_mat(CVImg);
//    mat2wxImage(CVImg);
    Refresh();
}



wxImage ImagePanel::wx_from_mat(cv::Mat &img) {
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

void ImagePanel::mat2wxImage(cv::Mat &cvImg) {
    try
    {
        // data dimension
        int w = cvImg.cols;
        int h = cvImg.rows;
        int size = w*h*3*sizeof(unsigned char);

        // allocate memory for internal wxImage data
        unsigned char* wxData = (unsigned char*) malloc(size);

        // the matrix stores BGR image for conversion
        cv::Mat cvRGBImg = cv::Mat(h, w, CV_8UC3, wxData);
        switch (cvImg.channels())
        {
            case 3: // 3-channel case: swap R&B channels
            {
                int mapping[] = {0,2,1,1,2,0}; // CV(BGR) to WX(RGB)
                mixChannels(&cvImg, 1, &cvRGBImg, 1, mapping, 3);
            } break;

        }

        image.Destroy(); // free existing data if there's any
        image = wxImage(w, h, wxData);
    }
    catch(...)
    {
    }
}
