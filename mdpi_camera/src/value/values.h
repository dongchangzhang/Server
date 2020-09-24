//
// Created by vhdsih on 20-1-18.
//

#ifndef MDPI_CAMERA_VALUES_H
#define MDPI_CAMERA_VALUES_H

#include <queue>
#include <string>
#include <vector>

#include <wx/wx.h>

#include "../data/Pixel.h"
#include "../data/GNC.h"
#include "../data/DataInjection.h"


// ---------- clear global values -----------------

void finalize();

// ----------- global values helper ---------------

bool read_gnc(GNC &gnc);
bool write_gnc(const GNC &gnc);

bool read_show_gnc(GNC &gnc);
bool write_show_gnc(const GNC &gnc);

bool read_send_information(wxImage &image, GNC &gnc, DataInjection &dij);
bool write_send_information(const wxImage &image, const GNC &gnc, const DataInjection &dij);

bool read_show_gnc_bytes(char *buffer, size_t &n_byte, size_t max_n_byte);
bool write_show_gnc_bytes(char *buffer, size_t n_byte);


bool read_1_gnc_byte(char &byte);
bool write_gnc_bytes(char *buffer, size_t n_byte);

bool read_1_control_byte(char &byte);
bool write_control_bytes(char *buffer, size_t n_byte);

bool read_camera_params(DataInjection &dij);
bool write_camera_params(const DataInjection &dij);
bool update_camera_params(const DataInjection &new_dij, int type_code);

bool read_show_info(size_t &n_gnc, double &x, double &y, double &z);
bool write_show_info(double x, double y, double z);

// ------------- global values --------------------


// device flag
bool& get_gnc_rs422_device_open_state();
bool& get_commands_rs422_device_open_state();


// counter
int& get_gnc_count_from_restart();
unsigned char& get_frame_count_from_restart();

// data
DataInjection& get_camera_ctrl_params();
Pixel*& get_mars_pixels_data();

// queue
std::queue<GNC>& get_gnc_queue_to_show();
std::queue<GNC>& get_gnc_queue_to_send();
std::queue<GNC>& get_gnc_queue_to_generate();
std::queue<unsigned char>& get_bytes_queue_recv_gnc_on_rs422();
std::queue<unsigned char>& get_bytes_queue_recv_ctrl_on_rs422();
std::queue<unsigned char>& get_bytes_queue_recv_gnc_on_rs422_to_show();

std::queue<wxImage>& get_image_queue_to_send();
std::queue<DataInjection>& get_camera_params_queue_to_send();


unsigned char*& get_pixels_buffer();
unsigned char*& get_sending_buffer();

wxMutex*& get_mutex_for_camera_params();

wxMutex*& get_mutex_for_gnc_queue();
wxMutex*& get_mutex_for_gnc_show_queue();
wxMutex*& get_mutex_for_camera_params_queue();

wxMutex*& get_mutex_for_gnc_rs422_device();
wxMutex*& get_mutex_for_camera_params_rs422_device();

wxMutex*& get_mutex_for_gnc_rs422_queue();
wxMutex*& get_mutex_for_gnc_rs422_to_show_queue();
wxMutex*& get_mutex_for_camera_params_rs422_queue();
wxMutex*& get_mutex_for_sending_image();
wxMutex*& get_mutex_for_camera_location();


double& get_gnc_x_to_show();
double& get_gnc_y_to_show();
double& get_gnc_z_to_show();
size_t& get_gnc_count_to_show();

std::string& get_ip_of_this_pc();
std::string& get_ip_of_remote_pc();

int& get_port_of_gnc();
int& get_port_of_camera_params();
int& get_port_of_image_sending();

#endif //MDPI_CAMERA_VALUES_H
