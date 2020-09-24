//
// Created by vhdsih on 2020/1/23.
//

#include "CamCtrlParser.h"
#include "../value/values.h"
#include "../data/Answer.h"
#include "../data/AnswerPooling.h"
#include "../ui/Home.h"

CamCtrlParser::CamCtrlParser(wxWindow *_handler) : ThreadBase(_handler), device(MODEMDEVICE_1) {

}

void *CamCtrlParser::Entry() {
    int buf_len = 0;
    const int BUF_SIZE = 256;
    unsigned char buf[BUF_SIZE];
    unsigned char last = 0x00, now, cmd_type = 0x00, pkg_len = 0, index = 0;
    unsigned short checksum = 0, checksum_cal = 0;
    bool in_dij = false, have_cmd = false, have_len = false;

    auto &mutex_rs422_device_ctrl = get_mutex_for_camera_params_rs422_device();
    DataInjection dij;

    while (!TestDestroy()) {

        // read data header
        if (!in_dij || !have_cmd || !have_len) {
            while (!read_1_control_byte(reinterpret_cast<char &>(now))) wxMilliSleep(10);
            if (!in_dij && last == HEAD1 && now == HEAD2) {
                in_dij = true;
            } else if (in_dij && !have_cmd) {
                cmd_type = now;
                if (cmd_type != TME_CMD && cmd_type != CHK_CMD &&
                    cmd_type != DIJ_CMD && cmd_type != TMP_CMD) {
                    in_dij = false;
                    have_cmd = false;
                    have_len = false;
                    last = now;
                    continue;
                }
                have_cmd = true;
            } else if (in_dij && have_cmd && !have_len) {
                pkg_len = now;
                have_len = true;
            }
            last = now;

        } else if (index <= pkg_len) { // <= :: len == real_len - 1
            while (index <= pkg_len && read_1_control_byte(reinterpret_cast<char &>(now))) {
                buf[index++] = now;
            }

        } else {
            checksum = 0;
            for (auto _i = 0; _i < 2; ++_i) {
                while (!read_1_control_byte(reinterpret_cast<char &>(now))) wxMilliSleep(10);
                checksum <<= 8u;
                checksum += now;
            }
            // cal checksum
            checksum_cal = cmd_type + pkg_len; // checksum -> add bytes
            for (int k = 0; k <= pkg_len; ++k) {
                checksum_cal += buf[k];
            }
            // checksum not yes, answer false
            if (checksum != checksum_cal) {
                std::cout << "Error checksum in device 1" << std::endl;
                Answer answer(cmd_type, 0xF2);
                buf_len = answer.get_buf(buf);
            } else if (cmd_type == DIJ_CMD) {
                // 数据注入
                std::cout << "DIJ update" << std::endl;
                int p = pkg_len;
                while (p >= 0) {
                    buf[p + 2] = buf[p];
                    --p;
                }
                buf[0] = cmd_type;
                buf[1] = pkg_len;

                while (!read_camera_params(dij)) wxMilliSleep(1);

                if (dij.parser(buf)) {
                    ((Home *) handler)->set_ctrl(dij);
                    auto *event = new wxThreadEvent(wxEVT_THREAD, ID_CTRL_THREAD_UPDATE);
                    wxQueueEvent(handler->GetEventHandler(), event);
                }
                while (!write_camera_params(dij)) wxMilliSleep(1);
                Answer answer(cmd_type, 0xF1);
                buf_len = answer.get_buf(buf);
            } else if (cmd_type == TME_CMD) {
                std::cout << "time check" << std::endl;
                // 时间码校时
                Answer answer(cmd_type, 0xF1);
                buf_len = answer.get_buf(buf);
            } else if (cmd_type == TMP_CMD) {
                std::cout << "temp check" << std::endl;
                // 温度
                Answer answer(cmd_type, 0xF1);
                buf_len = answer.get_buf(buf);
            } else if (cmd_type == CHK_CMD) {
                std::cout << "rotating check" << std::endl;
                // is rotating check pkg
                if (buf[0] == 0x5A && buf[1] == 0x5A) {
                    //buf_len = commands.get_dij_into_buffer(buf, 255);
                    AnswerPooling ap;
                    auto ngnc = get_gnc_count_from_restart();
                    while (!read_camera_params(dij)) wxMilliSleep(1);
                    buf_len = ap.get_pooling(buf, BUF_SIZE, dij, ngnc, 0);
                } else {
                    // not pkg
                    std::cout << "pkg format error" << std::endl;
                    buf_len = 0;
                }
            } else {
                std::cout << "error cmd" << std::endl;
                Answer answer(ERROR_CMD, 0xF4);
                buf_len = answer.get_buf(buf);
            }
            // answer
            while (mutex_rs422_device_ctrl->TryLock() == wxMUTEX_BUSY) {
                wxMilliSleep(10);
            }
            device.send(reinterpret_cast<char *>(buf), buf_len);
            mutex_rs422_device_ctrl->Unlock();
            last = 0x00;
            index = 0;
            pkg_len = 0;
            in_dij = false;
            have_cmd = false;
            have_len = false;
        }
    }
    return nullptr;
}

