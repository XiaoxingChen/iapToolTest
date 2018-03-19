cmake_minimum_required(VERSION 2.8)
PROJECT(iaptooltest)
AUX_SOURCE_DIRECTORY(. SRC_LIST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SRC_LIST ${SRC_LIST} ../iapDevice/udpIapDevice.cpp)
set_source_files_properties( ${SRC_LIST} PROPERTIES LANGUAGE "CXX" )

INCLUDE_DIRECTORIES(../ ../iapDevice/)
ADD_SUBDIRECTORY(../ iap)

ADD_EXECUTABLE(iaptooltest ${SRC_LIST})
TARGET_LINK_LIBRARIES(iaptooltest iap)
