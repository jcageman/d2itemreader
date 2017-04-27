Diablo 2 item reader
========


Introduction
------------

This library uses optical character recognition to find the item stats of diablo items. It uses the language autoit (www.autoitscript.com) to achieve this. For obtaining the character information the script used a very fast pixelsearch library to obtain the information.
This script could even be safely used on battle.net, although i didn't use it there. It also includes an example script, which automates the process of rerolling items for a private diablo 2 server.

Credits
------------
FastFind:
A small C++ library to quickly obtain information about pixels on the screen. In this github i use a slightly modified version of the library for additional speed for my use case.


https://github.com/FastFrench/FastFind

Diablo 2 OCR:
Thanks to the character definitions from this library the script got started, thanks for the ideas!
Initially this was too slow, but in combination with fastfind and additional improvements it now scans complete items under 0.1 second.
I also improved the correctness of the script, since it sometimes messed up characters.


https://github.com/ckcollab/diablo-2-autoit-ocr

How to Use
------------------

1. Download the latest version of autoit (www.autoitscript.com)
2. Run Diablo 2 in 800x600 without compromising quality.

itemreader.au3

This file is capable of reading item stats using the method GetItemStats(), it returns an array of all lines found. 
Currently it only searches in the left side of the screen due to the fact it was only used for the rollscript, which only needs to scan the left side of the screen.

rollscript.au3

This script was initially used for a private server in which it was possible to reroll items endlessly using the transmute button in the cube. It can easily be controlled using 'home' to start it 'pause' to pause it and 'end' to end it :)
It automates this process and its quite easy to add new stats to read or to decide which items to keep.
Make sure the Diablo 2 window is already active when you start the script.
