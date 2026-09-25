cmake_minimum_required(VERSION 3.28)
project(SQLitoSeguro LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

include(FetchContent)

FetchContent_Declare(
    FLTK
    GIT_REPOSITORY https://github.com/fltk/fltk.git
    GIT_TAG release-1.4.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(FLTK)

add_subdirectory(third_party/sqlite)

add_executable("SQLitoSeguro" ${SOURCES})
target_link_libraries("SQLitoSeguro" PRIVATE sqlite3)
target_link_libraries("SQLitoSeguro" PRIVATE fltk::fltk)
target_include_directories("SQLitoSeguro" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_compile_features("SQLitoSeguro" PRIVATE cxx_std_23)



#[===[
add_custom_command(
    TARGET "pwd_manager" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/test.db
        $<TARGET_FILE_DIR:pwd_manager>/test.db
)
]===]
install(TARGETS "SQLitoSeguro" RUNTIME DESTINATION bin)
