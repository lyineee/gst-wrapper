cmake_minimum_required(VERSION 3.20.0)
project(gst_wrapper VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)

add_library(gst_wrapper src/gst_wrapper.cpp src/gst_appsink.cpp src/gst_filesink.cpp src/gst_filesrc.cpp src/gst_sample_data.cpp src/gst_appsrc.cpp)
# target_link_libraries(gst-wrapper PRIVATE PkgConfig::gstreamer PkgConfig::gstreamer-app PkgConfig::gstreamer-video)
target_link_libraries(gst_wrapper PUBLIC PkgConfig::gstreamer PkgConfig::gstreamer-app PkgConfig::gstreamer-video)
target_include_directories(gst_wrapper 
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY 
include/${PROJECT_NAME}
    DESTINATION include
)

install(TARGETS 
gst_wrapper
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/install.cmake.in ${PROJECT_NAME}-config.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)


add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "lyineee")
include(CPack)
