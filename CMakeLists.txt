cmake_minimum_required(VERSION 3.14)
project(ServerGUI)

set(CMAKE_CXX_STANDARD 11)


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(wxWidgets_ROOT_DIR </usr/include/wx-3.0>)
    set(wxWidgets_CONFIGURATION mswu)
ENDIF()

find_package(OpenCV REQUIRED)
find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})

add_executable(ServerGUI main.cpp
        // ui
        ui/MyApp.cpp ui/MyApp.h ui/MyFrame.cpp ui/MyFrame.h ui/MyImgPanel.cpp ui/MyImgPanel.h
        ui/MyRecvThread.cpp ui/MyRecvThread.h ui/MyGNCThread.cpp ui/MyGNCThread.h ui/MyCmdThread.cpp ui/MyCmdThread.h
        // net
        net/Server.cpp net/Server.h net/Client.cpp net/Client.h
        // utils
        utils/Log.h utils/utils.h utils/utils.cpp
        // data
        data/RS422.cpp data/RS422.h data/Parser.cpp data/Parser.h
        data/Data.cpp data/Data.h data/GNC.cpp data/GNC.h data/LAN.cpp data/LAN.h  data/ImageToSend.cpp
        data/ImageToSend.h data/INFO.cpp data/INFO.h data/DATA_SEG.cpp data/DATA_SEG.h data/TimeCheck.cpp
        data/TimeCheck.h data/Pooling.cpp data/Pooling.h data/Answer.cpp data/Answer.h
        data/DataInjection.cpp data/DataInjection.h
        )

target_link_libraries(ServerGUI ${OpenCV_LIBS})
target_link_libraries(ServerGUI ${wxWidgets_LIBRARIES})
