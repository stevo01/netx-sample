#include "mqtt_task.h"
#include "logger.h"

#include "tx_api.h"
#include "nx_api.h"

#include "stdint.h"
#include "task_registry.h"

#define TOPIC_NAME                  "/device"
#define MESSAGE_STRING              "This is a message. "


static VOID my_disconnect_func(NXD_MQTT_CLIENT *client_ptr)
{
    TaskBase* task = TaskRegistry::Instance().FindTask(client_ptr);
    task->evt.set(MQTT_DISCONNECT_EVENT);
    return;
}

static VOID my_notify_func(NXD_MQTT_CLIENT* client_ptr, UINT number_of_messages)
{
    NX_PARAMETER_NOT_USED(number_of_messages);
    TaskBase* task = TaskRegistry::Instance().FindTask(client_ptr);
    task->evt.set(MQTT_NOTIFY_EVENT);
    return;
}

void MqttTask::init()
{
    Logger logger = Logger::Instance();
    
    uint32_t status;
    
    char mqtt_client_id[] ="mqtt-demo-client";

    /* Create MQTT client instance. */
    status = nxd_mqtt_client_create(&mqtt_client, 
                                    (char*)"mqtt_client", 
                                    mqtt_client_id, 
                                    strlen(mqtt_client_id),
                                    ip_ptr_, 
                                    pool_ptr_, 
                                    (void*)mqtt_client_stack, 
                                    sizeof(mqtt_client_stack), 
                                    MQTT_THREAD_PRIORTY, 
                                    NX_NULL, 
                                    0);
    
    if (status){
        logger.Log("Error in creating MQTT client: 0x%02x\n", status);
    }

    TaskRegistry::Instance().Register(&mqtt_client, this);

    status = nxd_mqtt_client_disconnect_notify_set(&mqtt_client, my_disconnect_func);
    if (status){
        logger.Log("Error nxd_mqtt_client_disconnect_notify_set() 0x%02x\n", status);
    }

    status = nxd_mqtt_client_receive_notify_set(&mqtt_client, my_notify_func);
    if (status){
        logger.Log("Error in nxd_mqtt_client_receive_notify_set() 0x%02x\n", status);
    }

    CreateConnection();
}

void MqttTask::run()
{
    Logger logger = Logger::Instance();
    logger.Log("MqttTask started\n");
    
    bool retv = false;
    uint32_t actual_flags;

    init();

    while (1) 
    {
        retv = evt.get(MQTT_ALL_EVENTS, &actual_flags, TX_WAIT_FOREVER);

        if(retv == true)
        {
    
            if( actual_flags & MQTT_T1_EVENT)
            {
                logger.Log("MQTT_T1_EVENT\n");
            }
            if( actual_flags & MQTT_DISCONNECT_EVENT)
            {
                logger.Log("MQTT_DISCONNECT_EVENT\n");
                CreateConnection();
            }
            if( actual_flags & MQTT_NOTIFY_EVENT)
            {
                logger.Log("MQTT_NOTIFY_EVENT\n");
                HandleIncomingMessage();
            }
        }
    }
}

void MqttTask::CreateConnection(void)
{
    uint32_t status;
    Logger logger = Logger::Instance();
    UINT keepalive_timer = 300;

    NXD_ADDRESS server_ip;
    server_ip.nxd_ip_address.v4 = IP_ADDRESS(10,10,0,1);
    server_ip.nxd_ip_version = NX_IP_VERSION_V4;
    


    logger.Log("server ip adress = %d.%d.%d.%d port=%d\n",
                (server_ip.nxd_ip_address.v4 >> 24) & 0xFF,
                (server_ip.nxd_ip_address.v4 >> 16) & 0xFF,
                (server_ip.nxd_ip_address.v4 >>  8) & 0xFF,
                (server_ip.nxd_ip_address.v4 >>  0) & 0xFF,
                1883);

    status = nxd_mqtt_client_connect(&mqtt_client, 
                                      &server_ip, 
                                      1883,
                                      keepalive_timer, 
                                      0, 
                                      NX_WAIT_FOREVER);
    

    if (status)
    {
        logger.Log("mqtt client connect failed: %d\n", status);
    }
    else {
        logger.Log("mqtt client connect passed.\n");

        /* Subscribe to the topic with QoS level 0. */
        status = nxd_mqtt_client_subscribe(&mqtt_client, (char*)TOPIC_NAME, strlen(TOPIC_NAME), 0);
        if (status){
            logger.Log("nxd_mqtt_client_subscribe failed: %d\n", status);
        }
        else {
            logger.Log("nxd_mqtt_client_subscribe passed. toppic: %s\n", TOPIC_NAME);
        }


    }
}

void MqttTask::HandleIncomingMessage(void)
{
    uint32_t status;
    uint8_t topic_buffer[128];
    uint8_t message_buffer[256];
    uint32_t topic_length;
    uint32_t message_length;
    Logger logger = Logger::Instance();
    
    status = nxd_mqtt_client_message_get(&mqtt_client, 
                                         topic_buffer, 
                                         sizeof(topic_buffer), 
                                         &topic_length, 
                                         message_buffer, 
                                         sizeof(message_buffer), 
                                         &message_length);
 
    if(status == NXD_MQTT_SUCCESS)
    {
        topic_buffer[topic_length] = 0;
        message_buffer[message_length] = 0;
        logger.Log("topic = %s, message = %s\n", topic_buffer, message_buffer);
    }

}




