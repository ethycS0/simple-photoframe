#ifndef SHARED_INC_RINGBUFFER
#define SHARED_INC_RINGBUFFER

#include "common-defines.h"

typedef struct ringbuffer_t {
        uint8_t* buffer;
        uint32_t mask;
        uint32_t read_index;
        uint32_t write_index;
} ringbuffer_t;

void ringbuffer_setup(ringbuffer_t* rb, uint8_t* buffer, uint32_t size);
bool ringbuffer_empty(ringbuffer_t* rb);
bool ringbuffer_write(ringbuffer_t* rb, uint8_t byte);
bool ringbuffer_read(ringbuffer_t* rb, uint8_t* byte);

#endif 
