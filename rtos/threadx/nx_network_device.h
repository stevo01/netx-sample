#pragma once

#include "nx_api.h"
#include <cstdint>
#include <stdint.h>
#include "logger.h"

class NetxNetworkDevice {
public:
    NetxNetworkDevice()
    {}

    uint32_t init(ULONG ip_addr, ULONG netmask,
              VOID (*driver)(NX_IP_DRIVER*),
                NX_IP* ip, NX_PACKET_POOL* pool)
    {
        uint32_t status;
        Logger logger = Logger::Instance();
        ip_=ip;
        pool_=pool;

        status = nx_ip_create(ip_, 
                              (char*)"netx-ip",
                              ip_addr, 
                              netmask,
                              pool_, 
                              driver,
                              ip_stack_, 
                              sizeof(ip_stack_),
                              1);
        if (status){
            logger.Log("error: nx_ip_create() failed\n");
            return status;
        }
        
        logger.Log("nx_ip_create() passed.\n");
        logger.Log("own ip adress = %d.%d.%d.%d \n",
                            (ip_addr >> 24) & 0xFF,
                            (ip_addr >> 16) & 0xFF,
                            (ip_addr >>  8) & 0xFF,
                            (ip_addr >>  0) & 0xFF);

        logger.Log("netmask  = %d.%d.%d.%d \n",
                            (netmask >> 24) & 0xFF,
                            (netmask >> 16) & 0xFF,
                            (netmask >>  8) & 0xFF,
                            (netmask >>  0) & 0xFF);

        status = nx_arp_enable(ip_, (void*)arp_cache_, sizeof(arp_cache_));

        if (status){
            logger.Log("nx_arp_enable() failed.\n");
            return status;
        }
        logger.Log("nx_arp_enable() passed.\n");

        
        status = nx_icmp_enable(ip_);
        if (status){  
            return status;
        }
        logger.Log("nx_icmp_enable() passed.\n");

        status = nx_tcp_enable(ip_);
        if (status){  
            return status;
        }
        logger.Log("nx_tcp_enable() passed.\n");

        status = nx_udp_enable(ip_);
        if (status){  
            return status;
        }
        logger.Log("nx_udp_enable() passed.\n");

        return NX_SUCCESS;
    }

    NX_IP* ip() const { return ip_; }
    NX_PACKET_POOL* pool() const { return pool_; }

private:
    NX_IP* ip_;
    NX_PACKET_POOL* pool_;

    UCHAR ip_stack_[2048];
    UCHAR arp_cache_[1024];
};
