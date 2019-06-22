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
        src/ui/App.cpp src/ui/App.h
        src/ui/MainFrame.cpp src/ui/MainFrame.h
        src/ui/ImagePanel.cpp src/ui/ImagePanel.h
        src/ui/ImageWorker.cpp src/ui/ImageWorker.h
        src/ui/GncWorker.cpp src/ui/GncWorker.h
        src/ui/CmdWorker.cpp src/ui/CmdWorker.h
        src/net/Server.cpp src/net/Server.h
        src/net/Client.cpp src/net/Client.h
        src/utils/Log.h
        src/utils/utils.h src/utils/utils.cpp
        src/data/RS422.cpp src/data/RS422.h
        src/data/Parser.cpp src/data/Parser.h
        src/data/Data.cpp src/data/Data.h
        src/data/GNC.cpp src/data/GNC.h
        src/data/LAN.cpp src/data/LAN.h
        src/data/ImageToSend.cpp src/data/ImageToSend.h
        src/data/INFO.cpp src/data/INFO.h
        src/data/DATA_SEG.cpp src/data/DATA_SEG.h
        src/data/TimeCheck.cpp src/data/TimeCheck.h
        src/data/Pooling.cpp src/data/Pooling.h
        src/data/Answer.cpp src/data/Answer.h
        src/data/DataInjection.cpp src/data/DataInjection.h
        src/ui/ImageFrame.cpp src/ui/ImageFrame.h)

target_link_libraries(ServerGUI ${OpenCV_LIBS})
target_link_libraries(ServerGUI ${wxWidgets_LIBRARIES})
