//
// Created by vhdsih on 20-1-21.
//

#ifndef MDPI_CAMERA_THREADBASE_H
#define MDPI_CAMERA_THREADBASE_H


#include <wx/wx.h>
class ThreadBase : public wxThread {
public:
    explicit ThreadBase(wxWindow *_handler);
    bool start_thread();
    bool stop_thread();
    wxWindow *handler;
};

#endif //MDPI_CAMERA_THREADBASE_H
