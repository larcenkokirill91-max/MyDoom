#!/bin/bash
g++ -std=c++17 game.cpp \
    imgui/imgui.cpp \
    imgui/imgui_demo.cpp \
    imgui/imgui_draw.cpp \
    imgui/imgui_tables.cpp \
    imgui/imgui_widgets.cpp \
    imgui/backends/imgui_impl_glfw.cpp \
    imgui/backends/imgui_impl_opengl3.cpp \
    -o MyImGuiApp \
    -Iimgui \
    -Iimgui/backends \
    $(pkg-config --cflags --libs glfw3 gl)
