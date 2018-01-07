---
title: Tutorial 3: Dissecting the Game (WIP)
---
Introduction
============
In parts 1 and 2 we introduced how snestistics is used to iteratively build an annotated disassembly of a SNES ROM. Now let's focus in on the parts of Battle Pinball we want to understand! An emulator with debugging features is an essential tool in this process. We'll be using [bsnes+](https://github.com/devinacker/bsnes-plus) for this task.

As previously mentioned we thought it'd be fun to translate the little text there is, as well as add SRAM serialization of the High Score Table. A high quality translation usually means a pretty invasive rewriting of the text system, preferably including a so called Variable Width Font routine to render typographically pleasing text. For this game we won't be so fancy, but look forward to the open source release of our Super Famicom Wars translation for more details on our take on that!

Title Screen
------------
The first Japanese text appears on the title screen.

![Title Screen](/images/tutorial-3/title_screen.png)

The logo is pretty neat as is with its English subtitle, so we probably won't look into replacing that. The list of credited copyright holders could be nice to make legible for English players, though. Let's see how its graphics are organized.

![Title Screen BG3 Tiles](/images/tutorial-3/title_screen_bg3_tiles.png)

Using bsnes+'s Tile Viewer we can see that the credits are not rendered with a font, but instead are composed using "pre-baked" 2-bit tiles. So all hacking needed to change this screen is to find how those tiles get transferred to VRAM. If the data is compressed, which is the norm on SNES, we will need to either reverse engineer the compression algorithm or make up more space in ROM and store a modified tile set uncompressed. 

In any case we can get back to this later when we've found some more interesting graphics to replace. Let's continue!

Intro "Cut Scene"
-----------------
Pressing `START` we arrive at a cut scene of sorts featuring a text writer.

![Text Writer](/images/tutorial-3/intro_text.png)

Taking a peek at the VRAM contents this time we discover that a 16x16 pixel Hiragana font is used, with some Katakana thrown in for good measure.

![Text Writer BG3 Tiles](/images/tutorial-3/intro_text_bg3_tiles.png)

There's only 48 glyphs available without increasing the number of tiles used. So, either we go for an ALL-CAPS latin replacement font, or we could try spilling over to what looks like the half-overwritten remains of an 8x8 pixel latin+katakana font. In any case the two tasks involved to translate this screen is:

- Replace the 16x16 pixel font with a custom latin one
- Reverse engineer the printing routines

Hacking text printing routines is essentially the heart of translation hacking, so let's start with that!

Intro Text Printer
------------------
We already found out, via bsnes+'s excellent Tile Viewer, that the tiles used for the intro text are assigned to the 2-bit per pixel BG3 layer. The next logical step is to find out where in VRAM the tile map data is allocated, and then follow the trail from there. 

![Text Writer BG3 Map](/images/tutorial-3/intro_text_bg3_map.png)

Opening the Tile Map viewer and switching it to view BG3 reveals that the map base address is `0xb800`. We can also note that the programmers have opted to use 8x8 tiles instead of 16x16 tiles. In theory the latter option would also work and would be slightly more convenient, but then the 8 pixel space between lines of text would need to be accomplished with HDMA or IRQs instead. 

I've also selected the upper left tile of the 5th glyph, and the inspector panel reveals that the address for that tile in VRAM is `0xbb9c`. Let's put a breakpoint on the next tile (`0xbba0`) that VRAM location and resume execution, to find out how the character values get there!

> Note: All VRAM locations are written as byte addresses.

![Text Writer VRAM Breakpoint](/images/tutorial-3/intro_text_vram_breakpoint.png)

A-ha! To my surprise DMA is not involved. Instead we discover a routine writing values to VRAM by directly banging register `$2218`, the VRAM data port. The little glimpse of code visible in the Disassembly view reveals that source data is located in RAM location `$0104` indexed by X, and that the VRAM destination is stored just before the tile data at address `$0102`.

To get a better view of this routine, let's go back to our initial snestistics disassembly, and look at the surrounding code.

Back to snestistics
-------------------

...