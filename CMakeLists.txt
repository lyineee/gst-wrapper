cmake_minimum_required(VERSION 3.20.0)
project(gst-wrapper VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)

add_library(gst-wrapper src/gst_wrapper.cpp src/gst_appsink.cpp src/gst_filesink.cpp src/gst_filesrc.cpp src/gst_sample_data.cpp src/gst_appsrc.cpp)
# target_link_libraries(gst-wrapper PRIVATE PkgConfig::gstreamer PkgConfig::gstreamer-app PkgConfig::gstreamer-video)
target_link_libraries(gst-wrapper PUBLIC PkgConfig::gstreamer PkgConfig::gstreamer-app PkgConfig::gstreamer-video)
target_include_directories(gst-wrapper PUBLIC include)


add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
