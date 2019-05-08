cmake_minimum_required(VERSION 3.14)
project(ServerGUI)

set(CMAKE_CXX_STANDARD 11)
set(wxWidgets_ROOT_DIR </usr/include/wx-3.0>)
set(wxWidgets_CONFIGURATION mswu)

find_package(OpenCV REQUIRED)
find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})

add_executable(ServerGUI main.cpp
        ui/MyApp.cpp ui/MyApp.h ui/MyFrame.cpp ui/MyFrame.h ui/MyImgPanel.cpp ui/MyImgPanel.h
        net/Server.cpp net/Server.h net/Client.cpp net/Client.h
        data/RS422.cpp utils/Log.h utils/utils.h utils/utils.cpp  data/RS422.h data/Parser.cpp data/Parser.h
        data/Data.cpp data/Data.h data/GNC.cpp data/GNC.h data/LAN.cpp data/LAN.h  data/ImageToSend.cpp
        data/ImageToSend.h data/INFO.cpp data/INFO.h data/DATA_SEG.cpp data/DATA_SEG.h data/TimeCheck.cpp
        data/TimeCheck.h data/Pooling.cpp data/Pooling.h data/Answer.cpp data/Answer.h data/DataInjection.cpp data/DataInjection.h ui/MyRecvThread.cpp ui/MyRecvThread.h ui/MyGNCThread.cpp ui/MyGNCThread.h ui/MyCmdThread.cpp ui/MyCmdThread.h)

target_link_libraries(ServerGUI ${OpenCV_LIBS})
target_link_libraries(ServerGUI ${wxWidgets_LIBRARIES})
