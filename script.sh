#!/bin/bash

if [[ "$1" == "clean" ]]; then
        echo "Cleaning both app and bootloader..."
        make -C app clean
        make -C bootloader clean
elif [[ "$1" == "json" ]]; then
        rm -f compile_commands.json

        bear -- make -C bootloader
        bear -- make -C app
        bear -- make -C flasher

        if [[ -f compile_commands.json ]]; then
                echo "compile_commands.json generated successfully."
        else
                echo "Error: Failed to generate compile_commands.json."
                exit 1
        fi

        echo "Build completed for both bootloader and app."

        echo "Cleaning build files after generating compile_commands.json..."
        make -C app clean
        make -C bootloader clean
        make -C flasher clean

elif [[ "$1" == "make" ]]; then
        echo "Building both app and bootloader..."
        make -C bootloader
        make -C app
elif [[ "$1" == "send" ]]; then
        st-info --probe
        st-flash --reset write app/firmware.bin 0x8000000
elif [[ "$1" == "all" ]]; then
        echo "Cleaning both app and bootloader..."
        make -C app clean
        make -C bootloader clean
        echo "Building both app and bootloader..."
        make -C bootloader
        make -C app
        st-info --probe
        st-flash --reset write app/firmware.bin 0x8000000
elif [[ "$1" == "dbg" ]]; then
        gdb-peda-arm ./app/firmware.elf
elif [[ "$1" == "ocd" ]]; then
        openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32f4x.cfg
else
        echo "Invalid argument. Use 'clean', 'json', 'make', 'all', 'dbg' or 'ocd'."
        exit 1
fi
