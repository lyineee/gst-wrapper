project(gst-wrapper-test VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


add_executable(test_wrapper main.cpp)
target_link_libraries(test_wrapper gst-wrapper)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
