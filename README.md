# Matilda Graphics Engine

MGE draws graphics with low RAM usage as a whole concept. It specifically designed to be used with microcontrollers that send image during the rendering process line by line to a display (many display controllers have their own memory to hold image). For RGB565, 320x240 screen you will need at least 640 bytes for a line buffer, 1kb for ~40 sprites buffer and several bytes for control context struct. If performance matters Matilda is capable of using 2 line buffers and 2 sprite buffers to maximize hardware utilization.

### Features

- written in C
- no dependencies (at all, even without a single malloc)
- mask sprites (bit/byte per pixel)
- pallete sprites (pallete per sprite, 256 colors per sprite max)
- Free for commercial and none commercial use (MIT license)

### Examples

All examples use Windows GDI for presenting the screen. To build everything use make.

1. Hello World - shows how to display bit mask and pallete sprites.
