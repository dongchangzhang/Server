//
// Created by vhdsih on 20-1-20.
//

#include "SendImage.h"

#include <utility>
#include "../net/Client.h"
#include "../value/values.h"
#include "../net/ImageSender.h"

SendImage::SendImage(wxWindow *_handler, std::string _ip, int _port)
    : ThreadBase(_handler), ip(std::move(_ip)), port(_port) {

}

// thread body
void *SendImage::Entry() {
    GNC gnc;
    wxImage image;
    DataInjection dij;
    while (!TestDestroy()) {
        if (!read_send_information(image, gnc, dij)) {
            wxMilliSleep(100);
            continue;
        }
        ImageSender sender((wxFrame*)handler, image, dij, gnc, ip, port);
        sender.send();
    }
    return nullptr;
}
