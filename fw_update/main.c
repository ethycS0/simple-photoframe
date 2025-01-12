#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAX_SIZE (0x669A)
#define KEY (0x4152)

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
        long fsize = (ftell(firmware)) / 16;
        if (fsize > (long)MAX_SIZE) {
                printf("Firmware size too large\n");
                return -1;
        }

        unsigned char total_packets[2];
        total_packets[0] = (unsigned char)(fsize & 0xFF);
        total_packets[1] = (unsigned char)((fsize >> 8) & 0xFF);

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

        unsigned char session_packet[4];
        unsigned char magic[] = {0x41, 0x59};

        session_packet[0] = magic[0];
        session_packet[1] = magic[1];
        session_packet[2] = total_packets[0];
        session_packet[3] = total_packets[1];

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
