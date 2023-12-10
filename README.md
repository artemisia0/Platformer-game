# PixelDream

## Description
This is a 2D terraria-like platformer game written in C++ 17 and SFML. It is completely free and open-source.
I have two short videos that illustrates how approximately this game looks like: [first](https://www.youtube.com/watch?v=YeXvfHxs7pc) and [second](https://www.youtube.com/watch?v=zkJQg86SbBU) one.

## Features
* Terrain generation using perlin noise (stb_perlin library used)
* More than 10 different kind of blocks (dirt, stone, gold)
* Smart character animation.
* Character AI (except player)
* Interpolated body movement (smooth)
* Many different states (menu, game, pause, game over, options...)
* Player has inventory (It is where blocks are stored)
* 3 kinds of enemies (Lizard, Orc, Wizard). Their behaviour is different
* 3 kinds of magic balls (such kind of weapon)
* Good user interface (transparent buttons/sliders)
* Proper music and sounds provided
* Shall work on most desktops (written and tested under linux)
* Parallax background
* Block creating/destroying
* Pickups
* Healthbox pickup that can be bought in workbench
* Workbench block that is used to craft or buy something
* Day/night cycle
* Good lighting system (ambient occlusion with respect to day time, torches)
* Efficient rendering but O(n*n) collision testing
* New blocks, weapons, characters can be added easily

## License
See LICENSE file.

## About
This project is cool for education. It was written from the scratch in less than 2 weeks.
I write my code in emacs, under the fedora linux. I use i3 window system and love XTerm.
