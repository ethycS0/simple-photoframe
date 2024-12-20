#include<stdio.h>
#include<fcntl.h>
#include<termios.h>

int main(void) {

        printf("\n +----------------------------------+");
        printf("\n |         Firmware Update          |");
        printf("\n +----------------------------------+");

        int fd;

        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
        if(fd == -1) {
                printf("Could Not Open UART Port");
                return -1;
        } else {
                printf("UART COM Port Opened Successfully");
        }

        struct termios UART_settings;
        
        // BAUD RATE
        cfsetspeed(UART_settings, B115200); 

        // 

        return 0;
}
