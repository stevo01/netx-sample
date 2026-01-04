#pragma once

#include "task_base.h"
#include "nxd_mqtt_client.h"


class TaskRegistry
{
    public:
        struct Entry
        {
            NXD_MQTT_CLIENT* client;
            TaskBase*        task;
        };

        // Configure maximum number of pairs you want to register
        static constexpr std::size_t kMaxEntries = 16;

        // Register a (client, task) pair. Returns true on success.
        bool Register(NXD_MQTT_CLIENT* client, TaskBase* task)
        {
            if (!client || !task) return false;

            LockGuard g(*this);

            // If already exists, update task pointer
            for (auto& e : entries_)
            {
                if (e.client == client)
                {
                    e.task = task;
                    return true;
                }
            }

            // Find empty slot
            for (auto& e : entries_)
            {
                if (e.client == nullptr)
                {
                    e.client = client;
                    e.task   = task;
                    return true;
                }
            }

            return false; // full
        }

        // Unregister by client. Returns true if removed.
        bool Unregister(NXD_MQTT_CLIENT* client)
        {
            if (!client) return false;

            LockGuard g(*this);

            for (auto& e : entries_)
            {
                if (e.client == client)
                {
                    e.client = nullptr;
                    e.task   = nullptr;
                    return true;
                }
            }
            return false;
        }

        // Look up task by client pointer. Returns nullptr if not found.
        TaskBase* FindTask(NXD_MQTT_CLIENT* client)
        {
            if (!client) return nullptr;

            LockGuard g(*this);

            for (auto& e : entries_)
            {
                if (e.client == client)
                    return e.task;
            }
            return nullptr;
        }

        // Optional convenience: singleton accessor
        static TaskRegistry& Instance()
        {
            static TaskRegistry inst;
            return inst;
        }

        // ---- Thread safety hook ----
        // If you want ThreadX protection, implement Lock()/Unlock() with TX_MUTEX.
        void Lock()   {}
        void Unlock() {}

    private:
        Entry entries_[kMaxEntries]{}; // zero-init => client/task = nullptr

        struct LockGuard
        {
            explicit LockGuard(TaskRegistry& r) : r_(r) { r_.Lock(); }
            ~LockGuard() { r_.Unlock(); }
            TaskRegistry& r_;
        };
};