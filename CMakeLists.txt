# 15.08.2024 12:20:07 Genrated by cbp2cmake V2.5.0 Finale X86 
project(mkguid LANGUAGES C)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set (CMAKE_INSTALL_PREFIX /usr )
add_executable(mkguid tools.c mkguid.c)
install(TARGETS mkguid RUNTIME DESTINATION bin)
