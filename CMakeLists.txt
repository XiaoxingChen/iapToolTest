cmake_minimum_required(VERSION 2.8)
aux_source_directory(. DIR_IAP)
set_source_files_properties( ${DIR_IAP} PROPERTIES LANGUAGE "CXX" )
add_library(iap ${DIR_IAP})
