#include "protocol.h"
#include"uart.h"
#include"system.h"

#define packet_ACK      (0x61)
#define packet_NAK      (0x6E)
#define session_ACK     (0x41)
#define session_NAK     (0x4E)

#define MAX_PACKETS     (10)
#define MAGIC           (0xA5A5)

#define RETRIES         (5)

void send_response(uint8_t response_value) {
        uart_write_byte(response_value);   
}

bool protocol_byte(uint8_t data) {
        uint8_t i = 0;
        while((data = uart_read_byte()) == 0x00 && i < RETRIES) {
                data = uart_read_byte();
                i++;
        }
        if(i >= RETRIES) {
                return false;
        }
        return true;
}

bool validate_session(session_t* ss) {
        bool lo;
        bool hi;

        lo = protocol_byte(ss->magic_no[1]);
        hi = protocol_byte(ss->magic_no[0]);

        if(lo == false || hi == false) {
                send_response(packet_NAK);
        }
         
        uint16_t magic = (ss->magic_no[0] << 8 | ss->magic_no[1]);
        if(magic != MAGIC) {
                send_response(session_NAK);
                return false;
        }

        lo = protocol_byte(ss->total_packets[1]);
        hi = protocol_byte(ss->total_packets[0]);
        
        if(lo == false || hi == false) {
                send_response(packet_NAK);
        }

        uint16_t total = (ss->total_packets[0] << 8 | ss->total_packets[1]);
        if(total > MAX_PACKETS) {
                send_response(packet_NAK);
                return false;
        }

        // crc check
        
        send_response(session_ACK);
        return true;
}
