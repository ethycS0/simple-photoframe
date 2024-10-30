# simple-photoframe

**simple-photoframe** is an STM32-based project designed to be a networked digital photo frame. The project is focused on building a custom stack from scratch, covering everything from a secure bootloader with AES encryption to a minimal RTOS with custom drivers for UART, graphics, and networking.

## Features

- **Custom Bootloader**: AES-encrypted validation for secure firmware loading over UART.
- **Minimal RTOS**: Custom RTOS for scheduling and task management.
- **Graphics Driver**: Display 480p images on a connected screen.
- **Networking Driver**: Receive and update images over the network.
- **UART Driver**: Custom UART implementation for firmware loading and communication.

## Project Goals

This project aims to enhance skills in embedded development, covering topics like low-level driver development, RTOS, networking, and cryptographic security.

## Project Structure

- `bootloader/` – Code for the custom bootloader with AES encryption.
- `rtos/` – Custom real-time operating system implementation.
- `drivers/` – Custom graphics, networking, and UART drivers.
- `firmware/` – Main application firmware for the photo frame.

## TODO List

### Bootloader
- [ ] **AES Encryption**: Implement AES encryption for secure firmware validation.
- [ ] **Firmware Loader**: Develop firmware loading functionality over UART.
- [ ] **Firmware Validation**: Integrate cryptographic checks for firmware integrity.

### RTOS
- [ ] **Scheduler**: Implement a basic round-robin or priority-based task scheduler.
- [ ] **Task Management**: Set up task creation, deletion, and priority management.

### Drivers
- [ ] **UART Driver**: Develop UART driver for bootloader and RTOS communication.
- [ ] **Graphics Driver**: Implement graphics driver to display images on 480p screen.
- [ ] **Networking Driver**: Create networking driver to fetch images over the network.

### Application
- [ ] **Photo Frame App**: Display fetched images on the screen, updated via network.

## Getting Started

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/yourusername/simple-photoframe.git
    cd simple-photoframe
    ```

2. **Set up Environment**: Install required toolchains for STM32 and any dependencies.

3. **Build and Flash**: Compile and upload using Makefiles.
