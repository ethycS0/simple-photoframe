#ifndef SHARED_INC_PROTOCOL
#define SHARED_INC_PROTOCOL

#include"common-defines.h"

typedef struct packet_t {
        unsigned int len : 4;
        unsigned int seqno : 4;
        uint8_t data[16];
        uint8_t crc[2];
} packet_t;

typedef struct session_t {
        uint8_t magic_no[2];
        uint8_t total_packets[2];
        uint8_t crc[2];
} session_t;


unsigned short generate_crc(unsigned char* data, int length); 

bool validate_session(session_t* ss);
bool validate_packet(packet_t* pk);

bool protocol_byte(uint8_t* data);

#endif

