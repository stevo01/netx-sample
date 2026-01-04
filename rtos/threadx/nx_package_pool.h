#pragma once

#include "nx_api.h"
#include <cstdint>
#include <sys/types.h>

#define     PACKET_SIZE     1536
#define     POOL_SIZE_1       ((sizeof(NX_PACKET) + PACKET_SIZE) * 256)


class NetxPacketPool {
public:
    NetxPacketPool()
        : pool_{} {}

    uint32_t init()
    {
        uint32_t status;
        status = nx_packet_pool_create( &pool_,
                                        (char*)"netx-packet-pool",
                                        PACKET_SIZE,
                                        pool_buffer_,
                                        POOL_SIZE_1
                                    );
        return status;
    }

    NX_PACKET_POOL* get() { return &pool_; }

private:
    NX_PACKET_POOL pool_;
    uint8_t pool_buffer_[POOL_SIZE_1]; 
};
