#include "protocol.h"
#include "uart.h"

#define packet_ACK (0x61)
#define packet_NAK (0x6E)
#define session_ACK (0x41)
#define session_NAK (0x4E)

#define MAX_PACKETS (0x669A)
#define MAGIC (0x4159)

#define RETRIES (15)
#define TIMEOUT (5000)

unsigned short generate_crc(unsigned char* data, int length) {
        unsigned short crc = 0x1d0f;
        while(length--) {
                crc ^= (unsigned short)(*data++) << 8;
                for(int i = 0; i < 8; i++) {
                        if(crc & 0x8000) {
                                crc = (crc << 1) ^ 0x1021;
                        } else {
                                crc <<= 1;
                        }
                }
        }
        return crc;
}


bool validate_session(session_t *ss) {



        ss->magic_no[0] = 0x00;
        uint16_t magic = 0;
        int i = 0;
        bool timeout;

        // Check Magic Number
        do {
                ss->magic_no[0] = ss->magic_no[1];
                timeout = uart_timeout(TIMEOUT);
                if (timeout != true) {
                        uart_write_byte(session_NAK);
                        return false;
                }

                ss->magic_no[1] = uart_read_byte();
                magic = (ss->magic_no[0] << 8 | ss->magic_no[1]);
                i += 1;

                if (i >= RETRIES) {
                        uart_write_byte(session_NAK);
                        return false;
                }
        } while (magic != MAGIC);

        // Check Total Packets

        timeout = uart_timeout(TIMEOUT);
        if (timeout != true) {
                uart_write_byte(session_NAK);
                return false;
        }
        ss->total_packets[0] = uart_read_byte();

        timeout = uart_timeout(TIMEOUT);
        if (timeout != true) {
                uart_write_byte(session_NAK);
                return false;
        }
        ss->total_packets[1] = uart_read_byte();

        if(((ss->total_packets[0] << 8) | ss->total_packets[1]) > MAX_PACKETS) {
                uart_write_byte(session_NAK);
                return false;
        }

        // Get CRC 
        
        timeout = uart_timeout(TIMEOUT);
        if (timeout != true) {
                uart_write_byte(session_NAK);
                return false;
        }
        ss->crc[0] = uart_read_byte();

        timeout = uart_timeout(TIMEOUT);
        if (timeout != true) {
                uart_write_byte(session_NAK);
                return false;
        }
        ss->crc[1] = uart_read_byte();


        // Generate crc

        unsigned char session_packet[4];
        session_packet[0] = ss->magic_no[0];
        session_packet[1] = ss->magic_no[1];
        session_packet[2] = ss->total_packets[0];
        session_packet[3] = ss-> total_packets[1];

        short gen_crc = generate_crc(session_packet, 4);

        if((unsigned short)((ss->crc[0] << 8) | ss->crc[1]) != gen_crc) {
                uart_write_byte(packet_NAK);
                return -1;
        }

        uart_write_byte(session_ACK);
        return true;
}
