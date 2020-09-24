//
// Created by vhdsih on 20-1-18.
//

#include "values.h"

void finalize() {
    auto *&ptr_pixel_data = get_mars_pixels_data();
    delete[] ptr_pixel_data;
    ptr_pixel_data = nullptr;

    auto *&ptr_pixel_buffer = get_pixels_buffer();
    delete[] ptr_pixel_buffer;
    ptr_pixel_buffer = nullptr;

    auto *&ptr_sending_buffer = get_sending_buffer();
    delete[] ptr_sending_buffer;
    ptr_sending_buffer = nullptr;

    auto *ptr_mutex_a = get_mutex_for_gnc_queue();
    delete ptr_mutex_a;

    auto *ptr_mutex_b = get_mutex_for_gnc_show_queue();
    delete ptr_mutex_b;

    auto *ptr_mutex_c = get_mutex_for_camera_params_queue();
    delete ptr_mutex_c;

    auto *ptr_mutex_d = get_mutex_for_gnc_rs422_device();
    delete ptr_mutex_d;

    auto *ptr_mutex_e = get_mutex_for_camera_params_rs422_device();
    delete ptr_mutex_e;

    auto *ptr_mutex_f = get_mutex_for_gnc_rs422_queue();
    delete ptr_mutex_f;

    auto *ptr_mutex_g = get_mutex_for_gnc_rs422_to_show_queue();
    delete ptr_mutex_g;

    auto *ptr_mutex_h = get_mutex_for_camera_params_rs422_queue();
    delete ptr_mutex_h;

    auto *ptr_mutex_i = get_mutex_for_sending_image();
    delete ptr_mutex_i;
}

bool read_gnc(GNC &gnc) {
    auto &lock = get_mutex_for_gnc_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    bool read_yes = false;
    auto &gncs = get_gnc_queue_to_generate();
    if (!gncs.empty()) {
        read_yes = true;
        gnc = gncs.front();
        gncs.pop();
        while (gncs.size() > 1) {
            gncs.pop();
        }
    }
    lock->Unlock();
    return read_yes;
}

bool write_gnc(const GNC &gnc) {
    auto &lock = get_mutex_for_gnc_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    auto &gncs = get_gnc_queue_to_generate();
    gncs.push(gnc);
    while (gncs.size() > 1) {
        gncs.pop();
    }
    lock->Unlock();
    return true;
}

bool read_show_gnc(GNC &gnc) {
    auto &lock = get_mutex_for_gnc_show_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    bool read_yes = false;
    auto &gncs = get_gnc_queue_to_show();
    if (!gncs.empty()) {
        read_yes = true;
        gnc = gncs.front();
        gncs.pop();
    }
    lock->Unlock();
    return read_yes;
}

bool write_show_gnc(const GNC &gnc) {
    auto &lock = get_mutex_for_gnc_show_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    auto &gncs = get_gnc_queue_to_show();
    gncs.push(gnc);
    while (gncs.size() > 1) {
        gncs.pop();
    }
    lock->Unlock();
    return true;
}

bool read_send_information(wxImage &image, GNC &gnc, DataInjection &dij) {
    auto &lock = get_mutex_for_sending_image();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    auto &gncs = get_gnc_queue_to_send();
    auto &dijs = get_camera_params_queue_to_send();
    auto &images = get_image_queue_to_send();
    bool read_yes = false;
    if (!images.empty()) {
        read_yes = true;
        gnc = gncs.front();
        dij = dijs.front();
        image = images.front();
        gncs.pop();
        dijs.pop();
        images.pop();
    }
    lock->Unlock();
    return read_yes;
}

bool write_send_information(const wxImage &image, const GNC &gnc, const DataInjection &dij) {
    auto &lock = get_mutex_for_sending_image();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }

    auto &gncs = get_gnc_queue_to_send();
    auto &dijs = get_camera_params_queue_to_send();
    auto &images = get_image_queue_to_send();

    gncs.push(gnc);
    dijs.push(dij);
    images.push(image);

    lock->Unlock();
    return true;
}

bool read_show_gnc_bytes(char *buffer, size_t &n_byte, size_t max_n_byte) {
    auto &lock = get_mutex_for_gnc_rs422_to_show_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    auto &bytes = get_bytes_queue_recv_gnc_on_rs422_to_show();
    n_byte = 0;
    while (!bytes.empty()) {
        buffer[n_byte++] = bytes.front();
        bytes.pop();
        if (n_byte >= max_n_byte) {
            break;
        }
    }
    lock->Unlock();
    return true;
}

bool write_show_gnc_bytes(char *buffer, size_t n_byte) {
    auto &lock = get_mutex_for_gnc_rs422_to_show_queue();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    auto &bytes = get_bytes_queue_recv_gnc_on_rs422_to_show();
    for (size_t i = 0; i < n_byte; ++i) {
        bytes.push(buffer[i]);
    }
    while (bytes.size() > 256) {
        bytes.pop();
    }
    lock->Unlock();
    return true;
}

bool read_1_gnc_byte(char &byte) {
    auto &lock = get_mutex_for_gnc_rs422_queue();
    auto &bytes = get_bytes_queue_recv_gnc_on_rs422();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    bool read_yes = false;
    if (!bytes.empty()) {
        read_yes = true;
        byte = bytes.front();
        bytes.pop();
    }
    lock->Unlock();
    return read_yes;
}
bool write_gnc_bytes(char *buffer, size_t n_byte) {
    auto &lock = get_mutex_for_gnc_rs422_queue();
    auto &bytes = get_bytes_queue_recv_gnc_on_rs422();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    for (size_t i = 0; i < n_byte; ++i) {
        bytes.push(buffer[i]);
    }
    lock->Unlock();
    return true;
}

bool read_1_control_byte(char &byte) {
    auto &lock = get_mutex_for_camera_params_queue();
    auto &bytes = get_bytes_queue_recv_ctrl_on_rs422();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    bool read_yes = false;
    if (!bytes.empty()) {
        read_yes = true;
        byte = bytes.front();
        bytes.pop();
    }
    lock->Unlock();
    return read_yes;
}

bool write_control_bytes(char *buffer, size_t n_byte) {
    auto &lock = get_mutex_for_camera_params_queue();
    auto &bytes = get_bytes_queue_recv_ctrl_on_rs422();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    for (size_t i = 0; i < n_byte; ++i) {
        bytes.push(buffer[i]);
    }
    lock->Unlock();
    return true;
}


bool read_camera_params(DataInjection &dij) {
    auto &lock = get_mutex_for_camera_params();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }

    dij = get_camera_ctrl_params();

    lock->Unlock();
    return true;
}

bool write_camera_params(const DataInjection &dij) {
    auto &lock = get_mutex_for_camera_params();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }

    get_camera_ctrl_params() = dij;

    lock->Unlock();
    return true;
}

bool update_camera_params(const DataInjection &new_dij, int type_code) {
    auto &lock = get_mutex_for_camera_params();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    short i, j, ni, nj;
    auto &dij = get_camera_ctrl_params();

    switch (type_code) {
        case UPDATE_WORK_MODE:
            dij.set_work_mode(new_dij.get_work_mode());
            break;
        case UPDATE_IMAGE_MODE:
            dij.set_image_mode(new_dij.get_image_mode());
            break;
        case UPDATE_WINDOW_ARGS:
            new_dij.get_windows_params(i, j, ni, nj);
            dij.set_window_args(i, j, ni, nj);
            break;
        case UPDATE_SAVING_INFO:
            dij.set_saving_info(new_dij.get_saving_mode(), new_dij.get_saving_path());
            break;
        case UPDATE_SENDING_MODE:
            dij.set_sending_mode(new_dij.should_send());
            break;
        default:
            ;
    }
    lock->Unlock();
    return true;
}

bool read_show_info(size_t &n_gnc, double &x, double &y, double &z) {
    auto &lock = get_mutex_for_camera_location();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    x = get_gnc_x_to_show();
    y = get_gnc_y_to_show();
    z = get_gnc_z_to_show();
    n_gnc = get_gnc_count_to_show();
    lock->Unlock();
    return true;
}
bool write_show_info(double x, double y, double z) {
    auto &lock = get_mutex_for_camera_location();
    if (lock->TryLock() == wxMUTEX_BUSY) {
        return false;
    }
    get_gnc_x_to_show() = x;
    get_gnc_y_to_show() = y;
    get_gnc_z_to_show() = z;
    get_gnc_count_to_show() += 1;
    lock->Unlock();
    return true;
}

// ------------- global values --------------------

bool& get_gnc_rs422_device_open_state() {
    static bool get_gnc_rs422_device_opened = false;
    return get_gnc_rs422_device_opened;
}

bool& get_commands_rs422_device_open_state() {
    static bool commands_rs422_device_opend = false;
    return commands_rs422_device_opend;
}

int& get_gnc_count_from_restart() {
    static int n_gnc = 0;
    return n_gnc;
}

unsigned char& get_frame_count_from_restart() {
    static unsigned char n_frame = 0;
    return n_frame;
}

DataInjection& get_camera_ctrl_params() {
    static DataInjection dij;
    return dij;
}

Pixel*& get_mars_pixels_data() {
    static auto *mars_pixels = new Pixel[GeoTiffPixelHeight * GeoTiffPixelWidth]();
    return mars_pixels;
}

std::queue<GNC>& get_gnc_queue_to_show() {
    static std::queue<GNC> gnc_queue_for_showing;
    return gnc_queue_for_showing;
}

std::queue<GNC>& get_gnc_queue_to_send() {
    static std::queue<GNC> gnc_queue_for_sending;
    return gnc_queue_for_sending;
}

std::queue<GNC>& get_gnc_queue_to_generate() {
    static std::queue<GNC> gnc_queue_for_generating;
    return gnc_queue_for_generating;
}

std::queue<unsigned char>& get_bytes_queue_recv_gnc_on_rs422() {
    static std::queue<unsigned char> bytes_queue_for_recv_gnc_on_rs422;
    return bytes_queue_for_recv_gnc_on_rs422;

}

std::queue<unsigned char>& get_bytes_queue_recv_ctrl_on_rs422() {
    static std::queue<unsigned char> bytes_queue_for_recv_ctrl_on_rs422;
    return bytes_queue_for_recv_ctrl_on_rs422;
}

std::queue<unsigned char>& get_bytes_queue_recv_gnc_on_rs422_to_show() {
    static std::queue<unsigned char> bytes_queue_for_recv_gnc_on_rs422_for_show;
    return bytes_queue_for_recv_gnc_on_rs422_for_show;
}

std::queue<wxImage>& get_image_queue_to_send() {
    static std::queue<wxImage> image_queue_to_send;
    return image_queue_to_send;
}

std::queue<DataInjection>& get_camera_params_queue_to_send() {
    static std::queue<DataInjection> camera_params_queue_to_send;
    return camera_params_queue_to_send;
}


unsigned char*& get_pixels_buffer() {
    static auto *pixels_buffer = new unsigned char[PIXELS_BUF_MAX_LEN]();
    return pixels_buffer;

}

unsigned char*& get_sending_buffer() {
    static auto *sending_buffer = new unsigned char[SEND_BUF_MAX_LEN]();
    return sending_buffer;
}

wxMutex*& get_mutex_for_camera_params() {
    static auto *mutex_for_camera_params = new wxMutex;
    return mutex_for_camera_params;
}

wxMutex*& get_mutex_for_gnc_queue() {
    static auto *mutex_for_gnc_queue = new wxMutex;
    return mutex_for_gnc_queue;
}

wxMutex*& get_mutex_for_gnc_show_queue() {
    static auto *mutex_for_gnc_show_queue = new wxMutex;
    return mutex_for_gnc_show_queue;
}

wxMutex*& get_mutex_for_camera_params_queue() {
    static auto *mutex_for_camera_params_queue = new wxMutex;
    return mutex_for_camera_params_queue;
}

wxMutex*& get_mutex_for_gnc_rs422_device() {
    static auto *mutex_for_gnc_rs422_device = new wxMutex;
    return mutex_for_gnc_rs422_device;
}

wxMutex*& get_mutex_for_camera_params_rs422_device() {
    static auto *mutex_for_camera_params_rs422_device = new wxMutex;
    return mutex_for_camera_params_rs422_device;
}

wxMutex*& get_mutex_for_gnc_rs422_queue() {
    static auto *mutex_for_gnc_rs422_queue = new wxMutex;
    return mutex_for_gnc_rs422_queue;
}

wxMutex*& get_mutex_for_gnc_rs422_to_show_queue() {
    static auto *mutex_for_gnc_rs422_to_show_queue = new wxMutex;
    return mutex_for_gnc_rs422_to_show_queue;
}

wxMutex*& get_mutex_for_camera_params_rs422_queue() {
    static auto *mutex_for_camera_params_rs422_queue = new wxMutex;
    return mutex_for_camera_params_rs422_queue;
}

wxMutex*& get_mutex_for_sending_image() {
    static auto *mutex_for_sending_image = new wxMutex;
    return mutex_for_sending_image;
}
wxMutex*& get_mutex_for_camera_location() {
    static auto *mutex_for_camera_location = new wxMutex;
    return mutex_for_camera_location;
}

double& get_gnc_x_to_show() {
    static double gnc_x = 0.0;
    return gnc_x;
}

double& get_gnc_y_to_show() {
    static double gnc_y = 0.0;
    return gnc_y;
}

double& get_gnc_z_to_show() {
    static double gnc_z = 0.0;
    return gnc_z;
}

size_t& get_gnc_count_to_show() {
    static size_t _n_gnc_to_show_ = 0;
    return _n_gnc_to_show_;
}

std::string& get_ip_of_this_pc() {
    static std::string ip_of_this_pc = "192.168.0.5";
    return ip_of_this_pc;
}

std::string& get_ip_of_remote_pc() {
    static std::string ip_of_remote_pc = "192.168.0.6";
    return ip_of_remote_pc;
}

int& get_port_of_gnc() {
    static int gnc_port = 5003;
    return gnc_port;
}
int& get_port_of_camera_params() {
    static int camera_params_port = 5002;
    return camera_params_port;
}
int& get_port_of_image_sending() {
    static int image_sending_port = 6000;
    return image_sending_port;
}
