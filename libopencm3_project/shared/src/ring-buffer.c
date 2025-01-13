#include"ring-buffer.h"

void ringbuffer_setup(ringbuffer_t* rb, uint8_t* buffer, uint32_t size) {
        rb->buffer = buffer;
        rb->read_index = 0;
        rb->write_index = 0;
        rb->mask = size - 1;
}

bool ringbuffer_empty(ringbuffer_t* rb) {
        return rb->read_index == rb->write_index;
}

bool ringbuffer_write(ringbuffer_t* rb, uint8_t byte) {
        uint32_t lread = rb->read_index;
        uint32_t lwrite = rb->write_index;
        
        uint32_t nwrite = (lwrite + 1) & rb->mask;

        if(nwrite == lread) {
                return false;
        }

        rb->buffer[lwrite] = byte;
        rb->write_index = nwrite;
        return true;
}

bool ringbuffer_read(ringbuffer_t* rb, uint8_t* byte) {
        uint32_t lread = rb->read_index;
        uint32_t lwrite = rb->write_index;

        if(lread == lwrite) {
                return false;
        }

        *byte = rb->buffer[lread];
        lread = (lread + 1) & rb->mask;
        rb->read_index = lread;

        return true;
}

