cmake_minimum_required(VERSION 3.13)
project (dialogtest C)

add_executable( dialogtest WIN32 dialog.c dialog.rc custom.c wm.c )
target_link_libraries( dialogtest comctl32 )
target_compile_definitions(dialogtest PUBLIC UNICODE _UNICODE)