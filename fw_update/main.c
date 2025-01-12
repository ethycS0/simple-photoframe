#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAX_SIZE (0x669A)

unsigned char* generate_crc(unsigned char* data, int length); 


unsigned char* generate_crc(unsigned char* data, int length) {
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
        unsigned char* result = (unsigned char*)malloc(2 * sizeof(unsigned char));
        if(result == NULL) {
                return NULL;
        }

        result[1] = (unsigned char)(crc & 0x00ff);
        result[0] = (unsigned char)((crc >> 8) & 0x00ff);

        return result;
}


int main(void) {

        printf("\n +----------------------------------+");
        printf("\n |         Firmware Update          |");
        printf("\n +----------------------------------+");
        printf("\n\n\n");

        // UART Connection Initiate

        int fd;
        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
        if (fd == -1) {
                printf("Could Not Open UART Port \n");
                return -1;
        } else if (!isatty(fd)) {
                printf("Not a tty Device. \n");
                close(fd);
                return -1;
        } else {
                printf("UART COM Port Opened Successfully \n");
        }

        // Firmware Select

        FILE *firmware;
        firmware = fopen("test", "r");
        if (firmware == NULL) {
                printf("Could not Open Firmware.\n");
                return -1;
        }
        fseek(firmware, 0L, SEEK_END);
        long fsize = ftell(firmware);
        long total_packets_bytes = (fsize + 15) / 16;
        if (total_packets_bytes > (long)MAX_SIZE) {
                printf("Firmware size too large\n");
                return -1;
        }

        unsigned char total_packets[2];
        total_packets[1] = (unsigned char)(total_packets_bytes & 0x00FF);
        total_packets[0] = (unsigned char)((total_packets_bytes >> 8) & 0x00FF);

        // UART configuration

        struct termios options;

        options.c_iflag =
            IGNPAR; // Ignore parity errors, as you do not use parity

        options.c_iflag &= ~(ICRNL | INLCR | BRKINT | IXON | IXOFF |
                             IXANY); // Disable newline translations, break
                                     // interrupts, and software flow control
        options.c_oflag = 0;         // Disable all output processing
        options.c_cflag =
            B115200 | CS8 | CLOCAL |
            CREAD; // 115200 baud, 8 data bits, no parity, 1 stop bit

        options.c_lflag = 0; // Raw mode, no signal handling, or canonical input

        options.c_cc[VMIN] = 1;  // Wait for at least 1 byte
        options.c_cc[VTIME] = 0; // No timeout

        tcsetattr(fd, TCSANOW, &options); // Apply changes immediately

        // Initialize Session
        ssize_t recv;
        ssize_t trns;
        unsigned char response[1];

        unsigned char session_packet[6];
        unsigned char magic[] = {0x41, 0x59};

        // CRC Calculation

        unsigned char* crc;

        session_packet[0] = magic[0];
        session_packet[1] = magic[1];
        session_packet[2] = total_packets[0];
        session_packet[3] = total_packets[1];

        for(int i = 0; i < 4; i++) {
                printf("%02X\n", session_packet[i]);
        }
        crc = generate_crc(session_packet, 4);
        if(crc == NULL) {
                printf("CRC Calculation Issue\n");
                return -1;
        }

        session_packet[4] = crc[0];
        session_packet[5] = crc[1];

        // Log Session
        printf("Session Initialization Log\n");
        printf("Magic: %02X%02X\n", session_packet[0], session_packet[1]);
        printf("Total Packets: %02X%02X\n", session_packet[2], session_packet[3]);
        printf("CRC: %02X%02X\n", session_packet[4], session_packet[5]);

        trns = write(fd, session_packet, sizeof(session_packet));
        if (trns == -1) {
                printf("Failed to write Session Packet\n");
                close(fd);
                return -1;
        }

        recv = read(fd, response, 1);
        if (recv == -1) {
                printf("Read Error\n");
                close(fd);
                return -1;
        }
        close(fd);
        fclose(firmware);
        return 0;
}
