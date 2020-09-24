//
// Created by vhdsih on 20-1-20.
//

#include "GncArgs.h"
#include "../value/const_values.h"
#include "../value/values.h"
#include "Home.h"

BEGIN_EVENT_TABLE(GncArgs, wxFrame)
    EVT_CLOSE(GncArgs::on_close)
    EVT_TIMER(TIMER1_ID , GncArgs::on_timer1)
    EVT_TIMER(TIMER2_ID , GncArgs::on_timer2)
END_EVENT_TABLE()

GncArgs::GncArgs(wxFrame *_handler)
        : handler(_handler), wxFrame(NULL, wxID_ANY, wxT("GNC内容详情"), wxDefaultPosition, wxSize(600, 500)), timer1(this, TIMER1_ID),
        timer2(this, TIMER2_ID) {



    auto *topSizer = new wxBoxSizer(wxHORIZONTAL);

    auto staticBox = new wxStaticBox(this, wxID_ANY, wxT("原始GNC信息[RS422]"));
    auto* staticSizer = new wxStaticBoxSizer(staticBox,wxVERTICAL);
    raw_gnc = new wxTextCtrl(this, wxID_ANY,
                                          wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                          wxTE_MULTILINE);

    auto staticBox3 = new wxStaticBox(this, wxID_ANY, wxT("GNC解析内容[RS422]"));
    auto* staticSizer3 = new wxStaticBoxSizer(staticBox3, wxVERTICAL);
    parsered_gnc = new wxTextCtrl(this, wxID_ANY,
                             wxEmptyString, wxDefaultPosition, wxDefaultSize,
                             wxTE_MULTILINE);

    staticSizer->Add(raw_gnc, 1, wxEXPAND | wxALL, 10);
    staticSizer3->Add(parsered_gnc, 1, wxEXPAND | wxALL, 10);

    topSizer->Add(staticSizer, 1, wxEXPAND | wxALL, 10);
    topSizer->Add(staticSizer3, 1, wxEXPAND | wxALL, 10);
    this->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    Centre();

    timer1.Start(250);
    timer2.Start(500);
}

void GncArgs::on_close(wxCloseEvent &event) {
    timer1.Stop();
    timer2.Stop();
    ((Home*)handler)->set_gnc_arg_window_state(false);
    Destroy();
}


void GncArgs::on_timer1(wxTimerEvent &event) {
    size_t n_byte = 0, max_n_byte = 500;

    if (!read_show_gnc_bytes((char*)buffer, n_byte, max_n_byte)) {
        return;
    }
    for (int i = 0; i < n_byte; ++i, ++n_bits) {
        if (n_bits != 0 && n_bits % 8 == 0) {
            raw_gnc->AppendText("\n");
        }

        if (buffer[i] <= 15) {
            raw_gnc->AppendText(wxString::Format(" 0%x ", buffer[i]));
        } else {
            raw_gnc->AppendText(wxString::Format(" %2x ", buffer[i]));
        }
    }

}

void GncArgs::on_timer2(wxTimerEvent &event) {
    GNC gnc;
    if (!read_show_gnc(gnc)) {
        return;
    }
    parsered_gnc->Clear();
    wxString gnc_parsered_info = wxString::Format(wxT("时间: %s\n俯仰： %lf\n偏航: %lf\n 滚动: %lf\nX: %lf\n Y: %lf\n Z: %lf\n"),
                                                  gnc.get_time(), gnc.get_pitch(), gnc.get_yaw(), gnc.get_roll(), gnc.get_x(), gnc.get_y(), gnc.get_z());
    parsered_gnc->AppendText(gnc_parsered_info);
}

