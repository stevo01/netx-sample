#pragma once

#include <cstdint>
#include <stdint.h>
#include "tx_api.h"
#include "nx_api.h"
#include "task_base.h"
#include "msg_buffer_tx.h"
#include "nxd_mqtt_client.h"

#define MQTT_THREAD_PRIORTY 10

constexpr const uint32_t MQTT_T1_EVENT               = 0x01;
constexpr const uint32_t MQTT_DISCONNECT_EVENT       = 0x02;
constexpr const uint32_t MQTT_NOTIFY_EVENT           = 0x04;
constexpr const uint32_t MQTT_ALL_EVENTS = (MQTT_T1_EVENT | MQTT_DISCONNECT_EVENT | MQTT_NOTIFY_EVENT );


constexpr const uint16_t  mqtt_task_stack_size = 4096;
constexpr const uint16_t  mqtt_client_stack_size = 4096;

    

class MqttTask : public TaskBase
{
    public:
        MqttTask(const char* name, uint32_t priority, TX_Queue *queue, NX_IP *ip_ptr, NX_PACKET_POOL *pool_ptr)
            : TaskBase(name, priority, 1, TX_NO_TIME_SLICE, mqtt_task_stack, mqtt_task_stack_size), 
            ip_ptr_(ip_ptr), pool_ptr_(pool_ptr), pqueue(queue)
        {

        }

    protected:
        void run() override; // To be implemented in .cpp file
        void init();
        void CreateConnection(void);
        //void PublishMessage(const char* topic, const char* message);
        void HandleIncomingMessage(void);

        uint8_t mqtt_task_stack[mqtt_task_stack_size];
        uint8_t mqtt_client_stack[mqtt_client_stack_size];
        NX_IP *ip_ptr_=nullptr;
        NX_PACKET_POOL *pool_ptr_=nullptr;
        TX_Queue *pqueue=nullptr;
        NXD_MQTT_CLIENT mqtt_client;
};