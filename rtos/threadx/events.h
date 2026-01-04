#pragma once

#include "tx_api.h"
#include <cstdint>

class Event {
public:
    Event() {
        UINT status = tx_event_flags_create(&event_group_, const_cast<CHAR *>("Event task"));
        if (status != TX_SUCCESS) {
            // handle error, optional: throw exception or assert
            abort();
        }
    }

    ~Event() {
        tx_event_flags_delete(&event_group_);
    }

    // Set bits in the event flag
    bool set(uint32_t flags) {
        uint32_t status;
        bool retv=false;
        
        status=tx_event_flags_set(&event_group_, flags, TX_OR);
        
        if(TX_SUCCESS==status){
            retv=true;
        }
        
        return retv;
    }

    // Clear bits in the event flag
    bool clear(uint32_t flags) {
        return tx_event_flags_set(&event_group_, ~flags, TX_AND) == TX_SUCCESS;
    }

    // Wait for any or all flags
    bool get(uint32_t flags, uint32_t *actual_flags, uint32_t timeout_ticks = TX_WAIT_FOREVER) {
        uint32_t status;
        bool retv=false;
        
        status = tx_event_flags_get(&event_group_, 
                                   flags, 
                                   TX_OR_CLEAR, 
                                   (ULONG*)actual_flags, 
                                   timeout_ticks);

        if(TX_SUCCESS==status){
            retv=true;
        }
        
        return retv;
    }

    // Peek flags without clearing
    bool peek(uint32_t flags, uint32_t& actual_flags, bool wait_all = false, uint32_t timeout_ticks = TX_NO_WAIT) {
        UINT get_option = wait_all ? TX_AND : TX_OR;
        return tx_event_flags_get(&event_group_, flags, get_option, (ULONG*)&actual_flags, timeout_ticks) == TX_SUCCESS;
    }

private:
    TX_EVENT_FLAGS_GROUP event_group_;
    uint32_t actual_flags;
};
