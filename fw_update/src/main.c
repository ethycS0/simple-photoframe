#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

int main(void) {

        printf("\n +----------------------------------+");
        printf("\n |         Firmware Update          |");
        printf("\n +----------------------------------+");
        printf("\n\n\n\n\n");

        int fd;

        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
        if (fd == -1) {
                printf("Could Not Open UART Port \n");
                return -1;
        } else if(!isatty(fd)) {
                printf("Not a tty Device. \n");
                return -1;
        } else {
                printf("UART COM Port Opened Successfully \n");
        }

        struct termios UART_settings;

        if(tcgetattr(fd, &UART_settings) == 0) {
                printf("UART Error \n");
                return -1;
        }
        
        UART_settings.c_iflag &= 


        close(fd);

        return 0;
}
