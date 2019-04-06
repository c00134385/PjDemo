cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../distribution)
#set(pathToDlib ${CMAKE_SOURCE_DIR}/src/main/)

#include(${pathToDlib}/dlib/cmake)

#3.设置第三方库头文件所在位置
include_directories(${CMAKE_SOURCE_DIR}/pjlib/include)
include_directories(${CMAKE_SOURCE_DIR}/pjlib-util/include)

#4.设置第三方库头库所在位置
#link_directories(${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/)
#4.导入第三方动态库
# 添加第三方动态库
add_library(pjlib
        STATIC
        IMPORTED)

# 设置第三方动态库属性（存储位置） armeabi-v7a
set_target_properties(pjlib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/pjlib/lib/libpj-arm-unknown-linux-androideabi.a)


add_library(native-lib SHARED
            com_jl_pjdemo_PjJni.cpp
            pjwrapper.cpp
            pjutils.cpp)

set_target_properties(native-lib PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY
                      ${distribution_DIR}/libs/${ANDROID_ABI})

find_library(log-lib log)
#find_library()



target_link_libraries(native-lib
#                      dlib::dlib
                        pjlib
                      ${log-lib})
#SET(CMAKE_BUILE_TYPE "RELEASE")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
#SET(CMAKE_BUILE_TYPE "DEBUG")
#SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
