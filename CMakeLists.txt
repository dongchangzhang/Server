cmake_minimum_required(VERSION 3.10)
project(Server)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
link_libraries(pthread)
add_executable(Server main.cpp net/Server.cpp net/Server.h net/Client.cpp net/Client.h data/RS422.cpp utils/Log.h
        data/RS422.h data/Parser.cpp data/Parser.h data/Data.cpp data/Data.h data/GNC.cpp data/GNC.h data/LAN.cpp data/LAN.h
        data/ImageToSend.cpp data/ImageToSend.h data/INFO.cpp data/INFO.h data/DATA_SEG.cpp data/DATA_SEG.h data/TimeCheck.cpp
        data/TimeCheck.h data/Pooling.cpp data/Pooling.h data/Answer.cpp data/Answer.h data/DataInjection.cpp data/DataInjection.h)
target_link_libraries(Server ${OpenCV_LIBS})
