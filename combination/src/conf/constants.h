//
// Created by vhdsih on 2019-06-15.
//

#ifndef PICTURECOLLAGEMAKER_CONSTANTS_H
#define PICTURECOLLAGEMAKER_CONSTANTS_H


enum {
    Btn_Begin,
    Btn_End,
    Btn_Save,
    Btn_Lead,
    Btn_GetImageDir,
    Btn_GetOutputDir,
    kThreadUpdateId,
};

#define IMG_TAIL ".jpeg"
#define INFO_TAIL ".dat"
#define RESULT_IMAGE "full_mars_image.png"

#define PI (3.1415926535897932346f)
#define SPEED  7.08821766e-5
#define NTHREAD 1

// camera
#define DEFAULT_PHOTO_H 3072
#define DEFAULT_PHOTO_W 4096
#define DEFAULT_CAMERA_FOV_HOR 53.14 //degree
#define DEFAULT_CAMERA_FOV_VER 41.1202256 // degree
#define DEFAULT_CAMERA_F_DISTANCE 22.52 // mm

#define WKT "PROJCS[\"SimpleCylindrical Mars\"," \
    "GEOGCS[\"GCS_Mars\",DATUM[\"D_Mars\",SPHEROID[\"Mars\","\
    "3396190,0]],PRIMEM[\"Reference_Meridian\",0],UNIT[\"degree\",0.0174532925199433]]," \
    "PROJECTION[\"Equirectangular\"],PARAMETER[\"latitude_of_origin\",0]," \
    "PARAMETER[\"central_meridian\",0],PARAMETER[\"standard_parallel_1\",0]," \
    "PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0]," \
    "UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]]]"

#define CANVAS_SHOW_H 576
#define CANVAS_SHOW_W 1152

#define GNC_BUF_LEN 80

#define CANVAS_H 4608
#define CANVAS_W 9216

#define ADFGEOTRANSFORM_0 -1.06694e+07
#define ADFGEOTRANSFORM_1 2315.42
#define ADFGEOTRANSFORM_2 0
#define ADFGEOTRANSFORM_3 5.33472e+06
#define ADFGEOTRANSFORM_4 0
#define ADFGEOTRANSFORM_5 -2315.42


#endif //PICTURECOLLAGEMAKER_CONSTANTS_H
