cmake_minimum_required(VERSION 3.5)
project(ServerGUI)

set(CMAKE_CXX_STANDARD 11)


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(wxWidgets_ROOT_DIR </usr/include/wx-3.0>)
    set(wxWidgets_CONFIGURATION mswu)
ENDIF()

find_package(OpenCV REQUIRED)
find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})

add_executable(ServerGUI src/main.cpp
        // src/ui
        src/ui/MyApp.cpp src/ui/MyApp.h src/ui/MyFrame.cpp src/ui/MyFrame.h src/ui/MyImgPanel.cpp src/ui/MyImgPanel.h
        src/ui/MyRecvThread.cpp src/ui/MyRecvThread.h src/ui/MyGNCThread.cpp src/ui/MyGNCThread.h src/ui/MyCmdThread.cpp src/ui/MyCmdThread.h
        // src/net
        src/net/Server.cpp src/net/Server.h src/net/Client.cpp src/net/Client.h
        // src/utils
        src/utils/Log.h src/utils/utils.h src/utils/utils.cpp
        // src/data
        src/data/RS422.cpp src/data/RS422.h src/data/Parser.cpp src/data/Parser.h
        src/data/Data.cpp src/data/Data.h src/data/GNC.cpp src/data/GNC.h src/data/LAN.cpp src/data/LAN.h src/data/ImageToSend.cpp
        src/data/ImageToSend.h src/data/INFO.cpp src/data/INFO.h src/data/DATA_SEG.cpp src/data/DATA_SEG.h src/data/TimeCheck.cpp
        src/data/TimeCheck.h src/data/Pooling.cpp src/data/Pooling.h src/data/Answer.cpp src/data/Answer.h
        src/data/DataInjection.cpp src/data/DataInjection.h
        )

target_link_libraries(ServerGUI ${OpenCV_LIBS})
target_link_libraries(ServerGUI ${wxWidgets_LIBRARIES})
