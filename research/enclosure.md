# Enclosure prototyping
This file deals with the prototyping of the enclosure. The research I've done to build the enclosure is detailed in the next subsections. Darryl was very helpful throughout the entire process (buy him a beer).

## Initial sketch
My initial idea was the following:
![initial-sketch](./images/initial-sketch.jpg)

### Finding a potential box as baseline
First, Edu pointed [Hammond website](https://www.hammfg.com) containing has several different types of enclosures that can be used in DIY projects. Taking the measures of the iPad as a reference, I found three boxes (list [here](https://docs.google.com/spreadsheets/d/1D_RBQ6R6dxn2w-e68LABvtBa7pAwvGjO6KwVCHRj6x4/edit?usp=sharing)) that could potentially work for me. After discussing them with Darryl, I decided to start paper prototyping using [this one here](https://www.hammfg.com/part/1441-22BK3CWW?referer=1234) as baseline.

The detailed measures of the chosen box are:
![enclosure-measures](./images/enclosure-measures.jpg)

## Iteration 1: Paper prototype
Based on the [official measures of that enclosure](https://www.hammfg.com/files/parts/pdf/1441-22BK3CWW.pdf), I did a paper prototype. First, I cut an letter-sized paper on the exact dimensions of the box. Then I have cut out holes in the size of the iPad screen, the buttons, and the potentiometers. The result is the following:

![paper-prototype-v1](./images/paper-prototype-v1.jpg)

## Iteration 2: Cardboard prototype
Based on the paper prototype, I've built the following cardboard prototype:

![cardboard-prototype-v1](./images/cardboard-prototype-v1.jpg)
![box3](./images/box3.jpg)
![box1](./images/box1.jpg)

This prototype provided a more concrete idea of the general look & feel. However, some important aspects remained unexplored. Namely:

### How to place the iPad under the top structure?
Here, I considered different solutions. One option would be [Jeffrey Stephenson's iPad TV box](http://slipperyskip.com/page38.html) (similar solution [here](https://www.youtube.com/watch?v=WuSLoM4i7Og)). Another would be to create a 'bed' on wood, and then cover it with a thin layer of plexiglass (such as [here](https://www.youtube.com/watch?v=Mjn0Njk-Nx8) and [here](https://www.youtube.com/watch?v=dWabuJQFBD0)). After a while, I decided to go with a sandwich of layers built with laser cutters, as shown in the image below:
![sandwich](./images/sandwich.jpg)

### How to hold still other hardware (i.e. FocusRite, USB hub, circuit) inside the enclosure?
Brainstormed solutions included: Sandwiching and against top and bottom parts; 3D printed case glued to bottom part; and wooden blocks to allow sandwich of the USB hub. In the end, the easiest solution was (again) sandwiching the FocusRite, and using some carving in the bottom layer to make it stand still. In my tests, the USB hub seems to stand still when all cables are connected.

### How to properly fix & separate bottom and upper layers?
Here I decided to use [standoffs/coupling nuts](https://www.mcmaster.com/#spacers-and-standoffs/=b5fq5j), the same strategy used by Ivan on its first Mitt prototype. I contacted him, and he said I could find those on [Home Depot Beaubien](https://www.homedepot.ca/en/home/categories/building-materials/hardware/fasteners/nuts/coupling-nuts.html). I went there and bought some.

As future reference, I was also able to find these standoffs on [Digikey](https://www.digikey.ca/products/en/hardware-fasteners-accessories/board-spacers-standoffs/582) Good models are [this one](https://www.digikey.ca/product-detail/en/wurth-electronics-inc/970500471/732-10662-ND/6174882) and [this one](https://www.digikey.ca/product-detail/en/wurth-electronics-inc/970500361/732-10661-ND/6174881), as they have approximately the height of the FocusRite. Their thread size is M3.

## Iteration 3: Paper prototype
Back to paper prototype to investigate the solutions proposed above:

![paper-prototype-v2](./images/paper-prototype-v2.jpg)

## Iteration 4: Cardboard prototype
The paper prototype led to a higher fidelity cardboard prototype. First, I replicated the paper cuts into the cardboard:

![cardboard-prototype-v2](./images/cardboard-prototype-v2.jpg)

Then, I successfully tested the 'sandwich' that will hold the iPad:

![ipad-sandwich](./images/ipad-sandwich.jpg)

This version also allowed me to [remove the protoboard](https://github.com/jeraman/LL-Experiment-01/blob/master/research/circuits.md), and test the electronics as they are going to be in the final prototype. That's what I've come up with:

![soldering-electronics](./images/soldering-electronics.jpg)

Then, I tried the standoffs:

![standoff-1](./images/standoff-1.jpg)
![standoff-2](./images/standoff-2.jpg)

Finally, I added the USB hub and the FocusRite as if this was the final looper:

![cardboard-v2-open](./images/cardboard-v2-open.jpg)

The result is still preliminary but seems quite solid, as you can see below:

![final-cardboard-v2](./images/cardboard-v2-top.jpg)
![final-cardboard-v2](./images/cardboard-v2-front.jpg)
![final-cardboard-v2](./images/cardboard-v2-back.jpg)

I loved this version, as it allowed to test almost every aspect of the final looper!

## Iteration 5:  Plexiglass prototype
![plexiglass-open](./images/plexiglass-open.jpg)

After the last cardboard prototype, I decided to move on to a plexiglass box, built with laser cutting. Regarding this, I found a bunch of useful resources at [ITP's fabrication courses webpage](https://itp.nyu.edu/fab/). Yves was very helpful during this iteration (buy him a beer too). Key take-aways from my laser cutting experience are:

- When creating a new file, remember to use landscape mode and RGB as color scheme;
- Maximum size of page (for the Speedy 300 we have at CIRMMT) should be 28.7 x 16.7 Inches.
- Remove all overlapping lines form your drawing;
- _Cutting color_ should be red (RGB: 255,0,0), and _engraving color_ should be black (RGB: 0,0,0);
- _Cutting line weight_ should be 0.18 or less; Regarding _engraving_, weight directly translates to 'strenght' of the engraving;
- When saving files, compatibility must be set to CS 5 or downwards;
- Put your drawing (and the material inside the printer) on the top left part.
- They only have CorelDraw and Inkscape at CIRMMT, so make sure you use one of these for the final printing.

Both Illustrator (original) and Corel Draw (converted) files are available [here](./images/cardboard-v2-back.jpg).

**Important**
> None of these versions are 100% complete and correct. The Illustrator one has the correct hole size for potentiometer, but misses the FocusRite holes made by Yves. The Corel version has the correct FocusRite holes, but potentiometer holes have a smaller size. Fix this problem for future versions.

The result is looking good!

![plexiglass-1](./images/plexiglass-1.jpg)
![plexiglass-3](./images/plexiglass-3.jpg)
![plexiglass-4](./images/plexiglass-4.jpg)
![plexiglass-6](./images/plexiglass-6.jpg)
![plexiglass-5](./images/plexiglass-5.jpg)

# Next Steps
- Add white paper in between layers 1 and 2 of the top paper (that holds the iPad);
- Buy smaller cables—one USB B to USB C; one USB to Firewire; one USB to Micro USB—for the inside;
- Add anti-slips plastics to the bottom.
