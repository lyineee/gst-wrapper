project(gst-wrapper-test VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


add_executable(test_wrapper main.cpp)
target_link_libraries(test_wrapper gst-wrapper)

add_executable(test_tee test_tee.cpp)
target_link_libraries(test_tee gst-wrapper)

add_executable(test_appsrc test_appsrc.cpp)
target_link_libraries(test_appsrc gst-wrapper)

add_executable(test_unlink test_unlink.cpp)
target_link_libraries(test_unlink gst-wrapper)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
