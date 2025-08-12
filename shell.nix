{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
   buildInputs = [
	pkgs.python3
	pkgs.gcc
	pkgs.nasm
	pkgs.uv
  pkgs.arduino
  pkgs.arduino-core
  pkgs.avrdude
   ];

   shellHook = ''
   	echo "Welcome to the MSong System build shell."
    export PATH=$PATH:${pkgs.arduino-core}/share/arduino/
	'';
}
