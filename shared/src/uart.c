#include "uart.h"
#include "ring-buffer.h"
#include "protocol.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#define BAUD_RATE (115200)
#define RING_SIZE (128)     // ~10ms latency

static ringbuffer_t rb = {0U};
static uint8_t data_buffer[RING_SIZE] = {0U};

static packet_t pk = {0};
static session_t ses = {0};

void usart2_isr(void) {
        const bool overrun_occured = usart_get_flag(USART2, USART_FLAG_ORE) == 1;
        const bool recieved_data = usart_get_flag(USART2, USART_FLAG_RXNE) == 1;

        if(recieved_data || overrun_occured) {
                if (ringbuffer_write(&rb, (uint8_t)usart_recv(USART2))) {
                        // Handle Failure
                }
        }
}

void uart_setup(void) {
        // Enabled Clock for the Peripheral
        ringbuffer_setup(&rb, data_buffer, RING_SIZE);
        rcc_periph_clock_enable(RCC_USART2);

        // Set USART flags
        usart_set_mode(USART2, USART_MODE_TX_RX);
        usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
        usart_set_databits(USART2, 8);
        usart_set_stopbits(USART2, 1);
        usart_set_baudrate(USART2, BAUD_RATE);
        usart_set_parity(USART2, 0);
        
        // Set UART interrupts
        usart_enable_rx_interrupt(USART2);
        nvic_enable_irq(NVIC_USART2_IRQ);

        // Enable
        usart_enable(USART2);

}

bool uart_session(void) {
        validate_session(&ses);
        return true;
}

void uart_write(uint8_t* data, const uint32_t length) {
         for(uint32_t i = 0; i < length; i++) {
                uart_write_byte(data[i]);
        }
}

void uart_write_byte(uint8_t data) {
        usart_send_blocking(USART2, (uint16_t)data);
}

uint32_t uart_read(uint8_t* data, const uint32_t length) {
        if(length == 0) {
                return 0;
        }

        for(uint32_t i = 0; i < length; i++) {
                if(!ringbuffer_read(&rb, &data[i])) {
                        return i;
                }
        }

        return length;
}

uint8_t uart_read_byte(void) {
        uint8_t byte = 0;
        uart_read(&byte, 1);
        return byte;
}

bool uart_data_available(void) {
        return !ringbuffer_empty(&rb);
}




