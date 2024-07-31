cmake_minimum_required(VERSION 3.13)
project (versioninfotest C)

add_executable( versioninfotest versioninfo.c versioninfo.rc )
target_link_libraries( versioninfotest version )