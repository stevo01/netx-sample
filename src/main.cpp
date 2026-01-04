#include "stdio.h"
#include "tx_api.h"
#include "nx_api.h"
#include "mqtt_task.h"
#include "logger.h"
#include "nx_package_pool.h"
#include "nx_network_device.h"

int main()
{
    printf("Hello NetX Sample Application!\n");

    tx_kernel_enter();

    return 0;
}   

TX_Queue *pMsgQueue;
#define PRO_TASK_PRIO 10

NetxPacketPool packetPool;
NX_IP ip_pp;
NetxNetworkDevice net;

extern "C" VOID  _nx_pcap_network_driver(NX_IP_DRIVER *driver_req_ptr);

void tx_application_define(void* first_unused_memory) {
    
    Logger logger = Logger::Instance();
    logger.Log("ThreadX started\n");

    nx_system_initialize();
    logger.Log("Netx started\n");
    
    if(packetPool.init() == NX_SUCCESS) {
        logger.Log("Packet pool created successfully\n");
    } else {
        logger.Log("Packet pool creation failed\n");
    }

    net.init(IP_ADDRESS(10,10,0,2),
             IP_ADDRESS(255,255,255,0),
             _nx_pcap_network_driver,
             &ip_pp,
             packetPool.get());

    pMsgQueue = new TX_Queue();
    MqttTask* es = new MqttTask("web server", PRO_TASK_PRIO, pMsgQueue, &ip_pp, packetPool.get());

    es->Start();

    logger.Log("MqttTask started\n");

}
