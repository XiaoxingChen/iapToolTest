cmake_minimum_required(VERSION 2.8)
aux_source_directory(. DIR_VFLASH)
set_source_files_properties( ${DIR_VFLASH} PROPERTIES LANGUAGE "CXX" )
add_library(vflash ${DIR_VFLASH})
