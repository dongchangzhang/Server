//
// Created by vhdsih on 20-1-21.
//

#include "ThreadBase.h"
ThreadBase::ThreadBase(wxWindow *_handler) : handler(_handler), wxThread(wxTHREAD_DETACHED) {

}

bool ThreadBase::start_thread() {
    this->Create();
    this->Run();
    return true;
}

bool ThreadBase::stop_thread() {
    this->Delete();
    return true;
}
