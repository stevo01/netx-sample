#pragma once

#include "tx_api.h"
#include <cstdint>
#include "events.h"

class TaskBase {
public:
    TaskBase(const char* name,
             uint32_t priority,
             uint32_t preemption_threshold,
             uint32_t time_slice,
             void* stack_ptr,
             uint32_t stack_size)
        : name_(name),
          priority_(priority),
          preemption_threshold_(preemption_threshold),
          time_slice_(time_slice),
          stack_ptr_(stack_ptr),
          stack_size_(stack_size),
          thread_created_(false){};

    // Prevent copying
    TaskBase(const TaskBase&) = delete;
    TaskBase& operator=(const TaskBase&) = delete;

    virtual ~TaskBase() = default;

    // Starts the ThreadX task (manual start)
    UINT Start() {

        UINT status = tx_thread_create(
            &thread_,
            const_cast<char*>(name_),
            &TaskBase::thread_entry,
            (ULONG)(this),
            //reinterpret_cast<ULONG>(this),
            //static_cast<ULONG>(reinterpret_cast<uintptr_t>(this)),
            stack_ptr_,
            stack_size_,
            priority_,
            preemption_threshold_,
            time_slice_,
            TX_DONT_START  // manual start
        );

        if (status == TX_SUCCESS) {
            thread_created_ = true;
            return tx_thread_resume(&thread_);
        }

        return status;
    }

    Event evt;

protected:
    // Override this method in derived classes
    virtual void run() = 0;

private:
    static void thread_entry(ULONG parameter) {
        auto* instance = reinterpret_cast<TaskBase*>(parameter);
        if (instance) {
            instance->run();
        }
    }

protected:
    const char* name_;
    uint32_t priority_;
    uint32_t preemption_threshold_;
    uint32_t time_slice_;
    void* stack_ptr_;
    uint32_t stack_size_;

private:
    TX_THREAD thread_;
    bool thread_created_;
};
