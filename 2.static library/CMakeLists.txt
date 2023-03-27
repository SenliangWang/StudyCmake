cmake_minimum_required(VERSION 3.15)

project ( hello_world_prj )

set( STATIC_LIB_SOURCES
    src/man.cpp
)

set( EXE_SOURCES
    src/main.cpp
)

add_library( hello_world_static_library STATIC 
    ${STATIC_LIB_SOURCES}
)

target_include_directories( hello_world_static_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

add_executable( hello_world_exe  ${EXE_SOURCES} )

target_include_directories( hello_world_exe
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries( hello_world_exe 
    PRIVATE 
        hello_world_static_library
)