//
// Created by idz on 2019/3/31.
//

#ifndef MARSSIMULATOR_DATAINJECTION_H
#define MARSSIMULATOR_DATAINJECTION_H


#include <zconf.h>

class DataInjection {
public:
    DataInjection() {
        _setting_window();
    }

    DataInjection(u_char buffer[]) {
        _setting_window();
    }

    ~DataInjection() {}

    u_char get_image_mode() const {
        return image_mode;
    }
    bool checksum() {
        return false;
    }
    void set_image_mode(u_char _image_mode) {
        image_mode = _image_mode;
    }

    void _setting_window();
    u_char get_work_mode() { return work_mode; }
    u_char get_expo_mode() { return expo_mode; }
    u_char get_image_mode() { return image_mode; }
    u_char* get_setting_windows() { return setting_window; }
    u_char* get_manual_expo() { return manual_expo; }
    u_char* get_auto_expo() { return auto_expo; }


private:
    u_char head1=0xeb;    // 包头标识
    u_char head2=0x90;    // 包头标识
    u_char command_type;    // 命令类型
    u_char valid_byte_num;    // 有效的字节数目

    // 需要仿真： y；   无需仿真： n
    u_char work_mode;    // 工作模式    y
    u_char expo_mode;    // 曝光模式    y
    u_char image_mode = 0x00;    // 图像模式    y
    u_char setting_window[9];    // 开窗设置    y
    u_char manual_expo[3];    // 手动曝光参数设置    y
    u_char auto_expo[5];    // 自动曝光参数设置    y
    u_char temperature[3];    // 温度    n
    u_char cmos_ele[3];    // cmos 电流    n
    u_char cmos_work[3];   // cmos 工作参数读写    n
    u_char output_request;    // 输出请求    n
    u_char SDRAM;    // 禁用控制    n
    u_char temp_change;   // 温度传感器切换    n

    u_char checksum_high;    // 累加和 高字节
    u_char checksum_low;    // 累加和 低字节
};


#endif //MARSSIMULATOR_DATAINJECTION_H
