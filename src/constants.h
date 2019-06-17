//
// Created by idz on 2019/4/3.
//

#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

#define GNC_ID 1
#define IMG_ID 2
#define LOAD_ID 3
enum {
    kThreadUpdateId,
};

// radius of mars - measurement is meters
#define RADIUS 3396190

// mars
#define SPEED  7.08821766e-5


// photo
#define PHOTO_WIDTH 4096
#define PHOTO_HEIGHT 3072

// camera
#define CAMERA_FOV_HOR 53.1
#define CAMERA_FOV_VER 41.1

// net
#define BUFFER_SIZE 4096 * 4
#define TIME_PORT 5001
#define INJECTION_POER 5002
#define GNC_PORT 5003
#define CMD_PORT 5024
#define INFO_PORT 5072
#define SEND_IMAGE_PORT 6000

#define TEST_ON_LOCALHOST

#define IP "192.168.0.5"

//#define PRINT_LOG_YES

#endif //SERVER_CONSTANTS_H
