cmake_minimum_required(VERSION 2.8)
PROJECT(iaptooltest_boost)

set(IAP_DEV_BOOST_DIR "D:/sdk2015/boost_1_63_0" CACHE PATH "Boost directory for IAP device")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINDOWS=0x0501")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
    message(STATUS "Set win")
endif()

INCLUDE(GetBoost)

if(NOT Boost_FOUND)
    message(STATUS "BOOST not found, test will not succeed!")
endif()

AUX_SOURCE_DIRECTORY(. SRC_LIST)


SET(SRC_LIST ${SRC_LIST} ../iapDevice/boostUdpIapDevice.cpp)
message(STATUS ${SRC_LIST})

INCLUDE_DIRECTORIES(${IAP_DEV_BOOST_DIR}/include/ ../ ../iapDevice/)
link_directories(${IAP_DEV_BOOST_DIR}/lib/)
ADD_SUBDIRECTORY(../ iap)

set_source_files_properties( ${SRC_LIST} PROPERTIES LANGUAGE "CXX" )

ADD_EXECUTABLE(iaptooltest_boost ${SRC_LIST})

if(WIN32)
    TARGET_LINK_LIBRARIES(iaptooltest_boost libboost_thread-vc140-mt-gd-1_63.lib)
elseif(UNIX)
    TARGET_LINK_LIBRARIES(iaptooltest_boost libboost_thread.a libboost_system.a)
endif()
TARGET_LINK_LIBRARIES(iaptooltest_boost iap)

