#include "protocol.h"
#include "system.h"
#include "uart.h"

#define packet_ACK (0x61)
#define packet_NAK (0x6E)
#define session_ACK (0x41)
#define session_NAK (0x4E)

#define MAX_PACKETS (0x7070)
#define MAGIC (0x5959)

#define RETRIES (15)
#define TIMEOUT (5000)

bool validate_session(session_t *ss) {
        ss->magic_no[0] = 0x00;
        uint16_t magic = 0;
        int i = 0;

        // Check Magic Number
        do {
                ss->magic_no[1] = ss->magic_no[0];
                uint64_t timeout = TIMEOUT + system_get_ticks();
                while(!uart_data_available()) {
                        if(system_get_ticks() > timeout) {
                                uart_write_byte(session_NAK);
                                return false;
                        }
                }

                ss->magic_no[0] = uart_read_byte();
                magic = (ss->magic_no[1] << 8 | ss->magic_no[0]);
                i += 1;

                if(i >= RETRIES) {
                        uart_write_byte(session_NAK);
                        return false;
                }
        } while (magic != MAGIC);


        // Check Total Packets


        uart_write_byte(session_ACK);
        return true;
}
