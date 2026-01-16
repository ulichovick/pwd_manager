cmake_minimum_required(VERSION 3.28)
project(pwd_manager LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_subdirectory(third_party/sqlite)

add_executable("pwd_manager" ${SOURCES})
target_link_libraries("pwd_manager" PRIVATE sqlite3)
target_include_directories("pwd_manager" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_compile_features("pwd_manager" PRIVATE cxx_std_17)

add_custom_command(
    TARGET "pwd_manager" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/test.db
        $<TARGET_FILE_DIR:pwd_manager>/test.db
)
install(TARGETS "pwd_manager" RUNTIME DESTINATION bin)
