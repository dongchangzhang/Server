//
// Created by vhdsih on 2020/1/23.
//

#include "GncParser.h"
#include "../value/values.h"

GncParser::GncParser(wxWindow *_handler) : ThreadBase(_handler) {

}

void *GncParser::Entry() {
    const int BUF_SIZE = 256;
    unsigned char buf[BUF_SIZE];
    unsigned char last = 0x00, now, index = 0;
    unsigned char cmd_type_bits[2], pkg_len_bits[2];
    unsigned short checksum = 0, checksum_cal = 0, tmp, pkg_len = 0;
    bool in_dij = false;
    int ctl_bits = 0, len_bits = 0, cmd_bits = 0;

    auto &mutex_rs422_device_gnc = get_mutex_for_gnc_rs422_queue();
    while (!TestDestroy()) {
        // read data header
        if (!in_dij || ctl_bits < 2 || len_bits < 2 || cmd_bits < 2) {
            if (!read_1_gnc_byte(reinterpret_cast<char &>(now))) {
                wxMilliSleep(1);
                continue;
            }
            if (!in_dij && last == HEAD1 && now == HEAD2) {
                in_dij = true;
            } else if (in_dij && ctl_bits < 2) {
                ++ctl_bits;
            } else if (in_dij && ctl_bits >= 2 && len_bits < 2) {
                pkg_len_bits[len_bits++] = now;
            } else if (in_dij && ctl_bits >= 2 && len_bits >= 2 && cmd_bits < 2) {
                pkg_len = pkg_len_bits[0];
                pkg_len = (pkg_len << 8) + pkg_len_bits[1] - 4; // -4 :: len - include cmd type and checksum 4B
                cmd_type_bits[cmd_bits++] = now;
                if (cmd_bits == 2 && (cmd_type_bits[0] != HGNC_CMD || cmd_type_bits[1] != LGNC_CMD)) {
                    index = 0;
                    last = 0x00;
                    pkg_len = 0;
                    ctl_bits = 0;
                    len_bits = 0;
                    cmd_bits = 0;
                    in_dij = false;
                    std::cout << "error cmd in dev2" << std::endl;
                }
            }
            last = now;
        } else if (index <= pkg_len) {
            while (index <= pkg_len && read_1_gnc_byte(reinterpret_cast<char &>(now))) {
                buf[index++] = now;
            }
            mutex_rs422_device_gnc->Unlock();
        } else {
            checksum = 0;
            for (auto _i = 0; _i < 2; ++_i) {
                while (!read_1_gnc_byte(reinterpret_cast<char &>(now))) wxMilliSleep(1);
                checksum <<= 8u;
                checksum += now;
            }
            // cal checksum
            checksum_cal = 0;
            tmp = cmd_type_bits[0];
            tmp <<= 8u;
            tmp += cmd_type_bits[1];
            checksum_cal ^= tmp; // checksum ^Bytes
            for (int k = 0; k <= pkg_len; k += 2) {
                tmp = buf[k];
                tmp <<= 8u;
                tmp += buf[k + 1];
                checksum_cal ^= tmp;
            }
            printf("checksum_real %x; checksum_cal %x\n", checksum, checksum_cal);

#ifdef DO_CHECKSUM
            // checksum not yes, answer false
            if (checksum != checksum_cal) {
                std::cout << "Error checksum on device A::gnc" << std::endl;
            } else {
#endif
                std::cout << "Checksum passed" << std::endl;
                std::cout << "GNC update by rs422" << std::endl;
                // 轨道信息
                GNC gnc;
                gnc.set_gnc_v2(buf, 8);
                if (gnc.is_right()) {
                    while (!write_show_info(gnc.get_x(), gnc.get_y(), gnc.get_z())) wxMilliSleep(1);
                    while (!write_gnc(gnc)) wxMilliSleep(1);
                    while (!write_show_gnc(gnc)) wxMilliSleep(1);
                } else {
                    std::cerr << "error gnc in dev 2" << std::endl;
                }
#ifdef DO_CHECKSUM
            }
#endif
            //reset
            index = 0;
            last = 0x00;
            pkg_len = 0;
            cmd_bits = 0;
            ctl_bits = 0;
            len_bits = 0;
            in_dij = false;
        }
    }
    return nullptr;
}
