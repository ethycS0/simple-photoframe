#!/usr/bin/env bash

ROOT_DIR=$(pwd)
DIRECTORIES=("bootloader" "app")

TEMP_COMPILE_COMMANDS="$ROOT_DIR/compile_commands.json"
rm -f "$TEMP_COMPILE_COMMANDS"

generate_compile_commands() {
    for dir in "${DIRECTORIES[@]}"; do
        echo "Running bear make in $dir..."
        (cd "$dir" && bear -- make)
    done
}

merge_compile_commands() {
    echo "Merging compile_commands.json files..."
    COMPILE_COMMANDS=()
    for dir in "${DIRECTORIES[@]}"; do
        if [[ -f "$dir/compile_commands.json" ]]; then
            COMPILE_COMMANDS+=("$dir/compile_commands.json")
        fi
    done

    if [[ ${#COMPILE_COMMANDS[@]} -gt 0 ]]; then
        jq -s 'add' "${COMPILE_COMMANDS[@]}" > "$TEMP_COMPILE_COMMANDS"
        echo "Merged compile_commands.json created in $ROOT_DIR"
    else
        echo "No compile_commands.json files found to merge."
    fi
}

generate_compile_commands
merge_compile_commands
