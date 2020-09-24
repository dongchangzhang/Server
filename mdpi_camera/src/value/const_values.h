//
// Created by vhdsih on 20-1-18.
//

#ifndef MDPI_CAMERA_CONST_VALUES_H
#define MDPI_CAMERA_CONST_VALUES_H

// wxWidgets ID
enum {
    ID_MENU_SETTING_IP_THIS,
    ID_MENU_SETTING_IP_REMOTE,
    ID_MENU_SETTING_PORT_GNC,
    ID_MENU_SETTING_PORT_DIJ,
    ID_MENU_SETTING_PORT_IMAGE,
    ID_MENU_INFO,
    ID_MENU_HELP,
    ID_BTN_RECV,
    ID_BTN_WIN,
    ID_BTN_TEST,
    ID_BTN_CAM,
    ID_BTN_EXIT,
    ID_BTN_BEGIN,
    ID_BTN_GNC_CON,
    ID_BTN_GNC_LOC,
    ID_RBTN_SAVE,
    ID_RBTN_SEE,
    ID_RBTN_IMODE,
    ID_RBTN_WMODE,
    ID_CTRL_THREAD_UPDATE,
    ID_IMAGE_THREAD_UPDATE,
    ID_SENDING_THREAD_UPDATE,
    ID_SENDING_THREAD_UPDATE_TEST,
    TIMER1_ID,
    TIMER2_ID,
    TIMER_GNC_LOC_ID,
};

/* flag */
#define DO_CHECKSUM 1
#define PRINT_INFO 1
// #define TEST_ON_LOCALHOST 1
// #define TEST_WITHOUT_RS422 1

/* rs422 */
#define HEAD1 0xeb
#define HEAD2 0x90
#define TME_CMD 0x1
#define CHK_CMD 0x2
#define DIJ_CMD 0x4
#define TMP_CMD 0x10
#define HGNC_CMD  0x00
#define LGNC_CMD  0x07
#define ERROR_CMD 0x0
#define BAUDRATE B115200

/* names of rs422 device */
#define MODEMDEVICE_1 "/dev/ttyL1" // gnc
#define MODEMDEVICE_2 "/dev/ttyL0" // camera params

/* values */
#define NTHREAD 8 // the number of thread
#define SPEED  7.08821766e-5
#define PI (3.1415926535897932346f)
#define WIN_WIDTH 864 // main window size w
#define WIN_HEIGHT 1152 // main window size h
#define VCDU_LEN 896 // sending udp lens :: VCDU
#define VCDU_DATA_LEN 878 // VCDU data-part
#define GeoTiffPixelWidth 18432 // mars image size w
#define GeoTiffPixelHeight 9216 // mars image size h
#define PIXELS_BUF_MAX_LEN (4096 * 2 * 3072) // buffer size for pixels - for sending
#define SEND_BUF_MAX_LEN  ((18 + 4000) * 3072 * 2 + 28) // for sending - v2

#define CAMERA_FOV_HORIZON 53.14f
#define CAMERA_FOCAL_DISTANCE 22.25f
#define CAMERA_FOV_VERTICAL 41.120226f
#define IMAGE_WIDTH_GENERATE int(4096.0 / 3)
#define IMAGE_HEIGHT_GENERATE int(3072.0 / 3)


/* data path */
#define DEFAULT_MARS_IMAGE "/usr/local/share/mars/mars.jpeg"
#define DEFAULT_MARS_PATH "/usr/local/share/mars/mars.tif"
#define DEFAULT_CAMERA_IMAGE "/usr/local/share/mars/camera.jpeg"
#define DEFAULT_MARS_PIXELS_PATH "/usr/local/share/mars/mars.bin"

/* update type code for camera params */
#define UPDATE_WORK_MODE 0x0
#define UPDATE_IMAGE_MODE 0x1
#define UPDATE_WINDOW_ARGS 0x2
#define UPDATE_SAVING_INFO 0x3
#define UPDATE_SENDING_MODE 0x4


/* mars coor */

#define WKT "PROJCS[\"SimpleCylindrical Mars\"," \
    "GEOGCS[\"GCS_Mars\",DATUM[\"D_Mars\",SPHEROID[\"Mars\","\
    "3396190,0]],PRIMEM[\"Reference_Meridian\",0],UNIT[\"degree\",0.0174532925199433]]," \
    "PROJECTION[\"Equirectangular\"],PARAMETER[\"latitude_of_origin\",0]," \
    "PARAMETER[\"central_meridian\",0],PARAMETER[\"standard_parallel_1\",0]," \
    "PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0]," \
    "UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]]]"


#define ADFGEOTRANSFORM_0 -1.06694e+07
#define ADFGEOTRANSFORM_1 1157.71
#define ADFGEOTRANSFORM_2 0
#define ADFGEOTRANSFORM_3 5.33472e+06
#define ADFGEOTRANSFORM_4 0
#define ADFGEOTRANSFORM_5 -1157.71

#endif //MDPI_CAMERA_CONST_VALUES_H
