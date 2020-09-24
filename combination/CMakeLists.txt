cmake_minimum_required(VERSION 3.5)
project(Combination)

set(CMAKE_CXX_STANDARD 11)

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

add_executable(Combination src/main.cpp
        src/ui/ImagePanel.cpp src/ui/ImagePanel.h
        src/ui/Image.cpp src/ui/Image.h
        src/ui/App.cpp src/ui/App.h
        src/ui/MainFrame.cpp src/ui/MainFrame.h
        src/ui/Constructer.cpp src/ui/Constructer.h
        src/conf/config.h src/conf/constants.h
        src/model/camera/CameraModel.cpp src/model/camera/CameraModel.h
        src/model/camera/perspectiveCamera.cpp src/model/camera/perspectiveCamera.h
        src/model/data/DataInjection.cpp src/model/data/DataInjection.h
        src/model/data/GNC.cpp src/model/data/GNC.h
        src/model/geo/Transform.cpp src/model/geo/Transform.h
        src/model/tools/cray.cpp src/model/tools/cray.h
        src/model/tools/csphere.cpp src/model/tools/csphere.h
        src/model/data/gvector3.cpp src/model/data/gvector3.h
        src/model/data/ImageData.cpp src/model/data/ImageData.h
        src/model/tools/intersectresult.h
        src/model/tools/utils.cpp src/model/tools/utils.h
        src/utils/tools.h src/utils/tools.cpp
        )

target_link_libraries(Combination ${OpenCV_LIBS})
target_link_libraries(Combination ${wxWidgets_LIBRARIES})
