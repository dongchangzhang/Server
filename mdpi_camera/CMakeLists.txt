cmake_minimum_required(VERSION 3.05)
project(mdpi_camera)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "Current Platform: Linux")
    include_directories("/usr/local/gdal/include")
    include_directories("/usr/include/eigen3")
    link_directories("/usr/local/gdal/lib")
ENDIF()

IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "Current Platform: Mac OSX")
    include_directories("/usr/local/opt/gdal/include")
    include_directories("/usr/local/opt/eigen/include/eigen3")
    link_directories("/usr/local/opt/gdal/lib")
ENDIF()

link_libraries(gdal)
link_libraries(pthread)
find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})

find_package(OpenCV REQUIRED)

add_executable(mdpi_camera src/main.cpp
        src/geo/Transform.cpp src/geo/Transform.h
        src/camera/perspectiveCamera.cpp src/camera/perspectiveCamera.h
        src/camera/CameraModel.cpp src/camera/CameraModel.h
        src/ui/App.cpp src/ui/App.h
        src/ui/Home.cpp src/ui/Home.h
        src/value/const_values.h
        src/value/values.cpp src/value/values.h
        src/data/Pixel.cpp src/data/Pixel.h
        src/ui/Image.cpp src/ui/Image.h
        src/ui/Test.cpp src/ui/Test.h
        src/ui/CamArgs.cpp src/ui/CamArgs.h
        src/ui/GncArgs.cpp src/ui/GncArgs.h
        src/ui/GncLoc.cpp src/ui/GncLoc.h
        src/net/Server.cpp src/net/Server.h
        src/net/Client.cpp src/net/Client.h
        src/net/RS422.cpp src/net/RS422.h
        src/data/VCDU.h
        src/data/GNC.cpp src/data/GNC.h
        src/data/INFO.cpp src/data/INFO.h
        src/data/DATA_SEG.cpp src/data/DATA_SEG.h
        src/data/TimeCheck.h
        src/data/Pooling.h
        src/data/Answer.h
        src/data/DataInjection.cpp src/data/DataInjection.h
        src/data/AnswerPooling.cpp src/data/AnswerPooling.h
        src/utils/cray.cpp src/utils/cray.h
        src/utils/csphere.cpp src/utils/csphere.h
        src/utils/gvector3.cpp src/utils/gvector3.h
        src/utils/intersectresult.h
        src/utils/directlight.cpp src/utils/directlight.h
        src/utils/color.cpp src/utils/color.h
        src/utils/Log.h
        src/utils/utils.cpp src/utils/utils.h src/thread/GncRecvByNet.cpp src/thread/GncRecvByNet.h src/thread/GncRecvByRs422.cpp src/thread/GncRecvByRs422.h src/thread/CamCtrlByNet.cpp src/thread/CamCtrlByNet.h src/thread/CamCtrlByRs422.cpp src/thread/CamCtrlByRs422.h src/thread/Camera.cpp src/thread/Camera.h src/thread/SendImage.cpp src/thread/SendImage.h src/thread/ThreadBase.cpp src/thread/ThreadBase.h src/data/ImageData.cpp src/data/ImageData.h src/thread/CamCtrlParser.cpp src/thread/CamCtrlParser.h src/thread/GncParser.cpp src/thread/GncParser.h src/net/ImageSender.cpp src/net/ImageSender.h src/data/GncShower.cpp src/data/GncShower.h)

target_link_libraries(mdpi_camera ${OpenCV_LIBS})
target_link_libraries(mdpi_camera ${wxWidgets_LIBRARIES})
