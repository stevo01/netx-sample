cmake_minimum_required(VERSION 3.16)
project(sample-net-app C CXX)

# set C and C++ Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(THREADX_ARCH linux)
set(THREADX_TOOLCHAIN gnu)
option(NXD_ENABLE_FILE_SERVERS OFF)

# add threadx common library
add_subdirectory(libs/threadx)
add_subdirectory(libs/netxduo)

##################################################################
# sample-app: sample application from threadx
##################################################################

# add threadx app
set(SAMPLE_APP_SRC
    # add threadx sample application
    src/main.cpp   
    #src/sample_threadx.c
)

# build the sample application
add_executable(${PROJECT_NAME} 
    ${SAMPLE_APP_SRC}
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./src
)

target_link_libraries(${PROJECT_NAME} 
        azrtos::threadx
        azrtos::netxduo
        )
