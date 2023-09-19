file(GLOB SOURCES "*.cpp")

add_library(lox_cpp ${SOURCES})

target_include_directories(lox_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)
