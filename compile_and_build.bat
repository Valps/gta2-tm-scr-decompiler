@echo off
meson setup build
ninja -C build
pause