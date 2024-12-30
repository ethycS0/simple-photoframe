# shell.nix

{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell
{
  nativeBuildInputs= with pkgs; [
    gcc-arm-embedded
    binutils
    gcc
    bear
    cmake
    clang-tools
    gnumake
    glibc_multi
  ];
}
