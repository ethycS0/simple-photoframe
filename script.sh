#!/bin/bash

if [[ "$1" == "clean" ]]; then
        echo "Cleaning both app and bootloader..."
        make -C app clean
        make -C bootloader clean
elif [[ "$1" == "json" ]]; then
        rm -f compile_commands.json

        bear -- make -C bootloader
        bear -- make -C app

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

elif [[ "$1" == "make" ]]; then
        echo "Building both app and bootloader..."
        make -C bootloader
        make -C app
else
        echo "Invalid argument. Use 'clean', 'json', or 'make'."
        exit 1
fi
