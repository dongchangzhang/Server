//
// Created by vhdsih on 2019-06-16.
//

#ifndef PICTURECOLLAGEMAKER_CONSTRUCTER_H
#define PICTURECOLLAGEMAKER_CONSTRUCTER_H

#include <wx/wx.h>
#include "../model/data/ImageData.h"
#include <vector>
#include <string>
#include <fstream>

class MainFrame;


class Constructer : public wxThread {
public:
    Constructer(MainFrame *_handler, int idx, ImageData &_canvas);

    bool stop();
    bool start();
    int statics(std::vector<std::string> &files);
    void *Entry() override;

private:
    int index;
    MainFrame *handler;
    ImageData canvas;
};


#endif //PICTURECOLLAGEMAKER_CONSTRUCTER_H
