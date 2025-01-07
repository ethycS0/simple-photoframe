{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "simple-photoframe";

  nativeBuildInputs = with pkgs; [
    gcc-arm-embedded
    binutils
    gcc
    bear
    clang-tools
    glibc_multi

    jq

    openocd
    gdb
    stlink

    screen
  ];

  shellHook = ''
        cat > .nix-zsh-setup <<EOF
    # Custom OpenOCD Command
    openocdstart() {
      openocd -f ${pkgs.openocd}/share/openocd/scripts/interface/stlink.cfg \
              -f ${pkgs.openocd}/share/openocd/scripts/target/stm32f4x.cfg
    }
    EOF
  '';
}
