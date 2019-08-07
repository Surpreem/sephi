#pragma once


namespace sephi::ipc {
    
    /**
     * Shared memory creation mode.
     */
    class ShmCreateOnly {};
    class ShmOpenOnly {};

    static ShmCreateOnly shm_create_only;
    static ShmOpenOnly shm_open_only;

    /**
     * Message queue creation mode.
     */
    class MqCreateOnly {};
    class MqOpenOnly {};

    static MqCreateOnly mq_create_only;
    static MqOpenOnly mq_open_only;

}
