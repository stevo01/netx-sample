#pragma once

#include "tx_api.h"
#include <cstdint>
#include <cassert>
#include "events.h"

class TX_Queue {
public:
    static constexpr size_t MESSAGE_SIZE = 64; // max size for Threadx 
    static constexpr size_t MESSAGE_COUNT = 10;
    static constexpr size_t MESSAGE_WORDS = MESSAGE_SIZE / sizeof(ULONG);
    static constexpr size_t QUEUE_MEMORY_WORDS = (MESSAGE_COUNT ) * MESSAGE_WORDS;

private:
    TX_QUEUE queue_;
    ULONG queue_memory[QUEUE_MEMORY_WORDS];  
    Event* p_evt;
    uint32_t event_flag;

public:
    TX_Queue() {
        memset(&queue_, 0, sizeof(queue_));
        UINT status = tx_queue_create(
            &queue_,
            const_cast<CHAR *>("TX_Queue"),
            MESSAGE_WORDS,
            queue_memory,
            sizeof(queue_memory)
        );
        assert(status == TX_SUCCESS && "tx_queue_create failed");
    }

    ~TX_Queue() {
        UINT status = tx_queue_delete(&queue_);
        assert(status == TX_SUCCESS && "tx_queue_delete failed");
    }

    bool push(const uint8_t (&data)[64]) {
        
        bool retv = tx_queue_send(&queue_, (void*)data, TX_NO_WAIT) == TX_SUCCESS;
        RaiseEvent();
        return retv; 
    }

    bool pop(uint8_t (&dest)[64]) {
        return tx_queue_receive(&queue_, (void*)dest, TX_WAIT_FOREVER) == TX_SUCCESS;
    }

    void clear() {
        tx_queue_flush(&queue_);
    }

    uint8_t isEmpty() const {
        ULONG enqueued = 0;
        uint8_t retv = 0;
        UINT status = tx_queue_info_get(const_cast<TX_QUEUE*>(&queue_), nullptr, &enqueued, nullptr, nullptr, nullptr, nullptr);
        if(status == TX_SUCCESS){
            if(enqueued == 0) {
                retv = 1;
            }
        }
        return retv;
    }

    uint8_t isFull() const {
        ULONG enqueued = 0;
        UINT status = tx_queue_info_get(const_cast<TX_QUEUE*>(&queue_), nullptr, &enqueued, nullptr, nullptr, nullptr, nullptr);
        return (status == TX_SUCCESS && enqueued >= MESSAGE_COUNT) ? 1 : 0;
    }

    uint8_t isNotFull() const {
        return !isFull();
    }

    uint8_t size() const {
        ULONG enqueued = 0;
        UINT status = tx_queue_info_get(const_cast<TX_QUEUE*>(&queue_), nullptr, &enqueued, nullptr, nullptr, nullptr, nullptr);
        return (status == TX_SUCCESS) ? static_cast<uint8_t>(enqueued) : 0;
    }

    void RaiseEvent(void) {
        if (p_evt != nullptr) {
            p_evt->set(event_flag);
        }
    }

    void RegisterEvent(Event* evt, uint32_t event_flag) {
        this->p_evt = evt;
        this->event_flag = event_flag;
    }
};
