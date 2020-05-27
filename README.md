# Bruh WM

A lightweight window manager made width GDK/GTK.

## Setup and running

Since the project is in super early development stages it is recommended to run it on a nested X Server session using Xephyr. After having installed Xephyr, simply run:

`Xephyr -ac -screen 800x600 :2 &`

This will start a nested X Server session in the background, enabling you to test the window manager without damaging your current stable desktop environment.

In order to build the project you first need to initialize the Meson build system. To setup Meson, simply run this command in the root of the project:

`meson builddir`

With the `builddir` created, to build and run the application simply move into it and run:

`ninja && ./bruh-wm`