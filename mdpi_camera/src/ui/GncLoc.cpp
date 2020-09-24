//
// Created by vhdsih on 20-1-20.
//

#include "GncLoc.h"
#include "Image.h"
#include "../value/values.h"
#include "../data/GncShower.h"
#include "Home.h"

BEGIN_EVENT_TABLE(GncLoc, wxFrame)
    EVT_CLOSE(GncLoc::on_close)
    EVT_TIMER(TIMER_GNC_LOC_ID , GncLoc::on_timer)
END_EVENT_TABLE()

GncLoc::GncLoc(wxFrame* _handler)
        : handler(_handler), wxFrame(NULL, wxID_ANY, wxT("相机相对位置"), wxDefaultPosition, wxSize(512, 384)),
        timer(this, TIMER_GNC_LOC_ID) {

        wxBoxSizer *topSizer=new wxBoxSizer(wxVERTICAL);

    wxImage image = shower.get_ret();

    img = new wxImagePanel(this, image, wxBITMAP_TYPE_JPEG, 1, 1);

    img->SetBackgroundColour(*wxBLACK);

    topSizer->Add(img, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 2);

    this->SetSizer(topSizer);

    Centre();

    timer.Start(300);
}

void GncLoc::on_close(wxCloseEvent &event) {
    timer.Stop();
    ((Home*)handler)->set_gnc_loc_window_state(false);
    Destroy();
}

void GncLoc::on_timer(wxTimerEvent &event) {
    int n_try = 0;
    double x, y, z;
    size_t n_gnc_to_show;

    while (!read_show_info(n_gnc_to_show, x, y, z)) {
        ++n_try;
        wxMicroSleep(10);
        if (n_try > 10) return;
    }

    if (_n_gnc_to_show_ >= n_gnc_to_show) {
        return;
    } else {
        _n_gnc_to_show_ = n_gnc_to_show;
    }
    std::cout << " ----------------------------- add " << x << " " << y << z << std::endl;
    auto image = shower.add(x, y, z);
    img->update_v2(image);
}
