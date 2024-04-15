cmake_minimum_required(VERSION 3.27)


add_library(${PROJECT_NAME} RealtimeServer.cpp ClientSocket.cpp Game.cpp EventHandler.cpp PackageFromPlayer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(sockpp REQUIRED)
target_link_libraries(${PROJECT_NAME} Sockpp::sockpp)


target_link_libraries(${PROJECT_NAME} object_threadsafe-lib)
target_link_libraries(${PROJECT_NAME} ByteStream)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)