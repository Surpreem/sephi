#pragma once


namespace sephi::ipc {
    
    /**
     * Shared memory creation mode.
     */
    class ShmCreateOnly {};
    class ShmOpenOnly {};

    static ShmCreateOnly shm_create_only;
    static ShmOpenOnly shm_open_only;

}
