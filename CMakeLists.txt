cmake_minimum_required(VERSION 3.16)
project(sample-net-app C CXX)

# set C and C++ Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(THREADX_ARCH linux)
set(THREADX_TOOLCHAIN gnu)
option(NXD_ENABLE_FILE_SERVERS OFF)
option(NX_HTTP_NO_FILEX ON)

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
    #src/test.cpp
    src/mqtt_task.cpp
    #src/nx_ram_network_driver.c
    src/nx_pcap_network_driver.c
    libs/netxduo/addons/mqtt/nxd_mqtt_client.c
)

# build the sample application
add_executable(${PROJECT_NAME} 
    ${SAMPLE_APP_SRC}
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ./src
        ./rtos/threadx
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        azrtos::threadx
        azrtos::netxduo
        pcap
        )

  target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    -DNX_PCAP_SOURCE_NAME=\"veth_netx\"
)

