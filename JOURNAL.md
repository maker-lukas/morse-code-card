---
title: "Morse Code Card"
github: "https://github.com/maker-lukas/morse-code-card"
description: "A credit card sized morse code trainer"
created_at: "2026-09-1"
---

# August 30: Schematic

At first i decided on the MCU that I was gonna use I first tought of some light ATMega or similar as this project does not need much power but when i discovered that the CH32 has the Touch Key peripheral GPIO I emidiatly chose the CH32V203DS0 (from the tutorial) this was easy to wire everything as its a really simple chip

- Added Boot/Reset buttons
- Used TestPoints for 4 programing pins with SWDIO/SWCLK + 3v3, GND
- Used a Coincell battery and as it runs in the same voltage as the CH32 needs i can wired it directly to the MCU
- Added a Speaker symbol for the piezo as it acts as one
- Added the 2 decoupling capacitors

After this I had to move onto something a bit harder and took me quite a bit longer which was Charlieplexing the 30 LEDs, the Charlieplexing concept was completly new to me so I did a fair amount of research which was not really enjoyable as I was having a really hard time understanding everything especially like scaling Charlieplexing, I watched tons of videos, read wikipedia and guides, asked Claude, And nothing clicked what worked was this [Instructables](https://www.instructables.com/Charlieplexing-Made-Easy-and-What-It-Even-Means/) and from there I was able to create the Charlieplexing Schematic!

And everything looks like this!

![Schematic](https://user-cdn.hackclub-assets.com/01a06d2c-cc45-75bf-9aaf-14fda54aa873/Screenshot_20260831_113423.png)
![Charlieplexing](https://user-cdn.hackclub-assets.com/01a06d2d-20d1-7af8-bbf1-5fc9327f729f/Screenshot_20260831_113439.png)
![MCU](https://user-cdn.hackclub-assets.com/01a06d2d-008c-7544-a5c1-6a811d526fce/Screenshot_20260831_113430.png)

**Total time spent: 4h**

# August 31: Morse tree + PCB Layout

As my schematic was already basically done I moved on to the next thing i tought would be the best which was to design the Morse Code Tree that I would use in the PCB. And this was also prety hard I firstly tried to understand the "math" behind the tree but i was not able to find / understand the arm scaling and angles so what i did is find a tree that i like IN .svg which i found one specifically [this](https://commons.wikimedia.org/wiki/File:Morse-code-tree.svg) one I used it specifically because of the size it was a good size to fit in the card I still striped the 5th level which contains numbers and a few symbols i did this because then it would simply be too big to fit in the card. 

Then my way to replicate this was look at the .SVG file get an LLM (opencode) to read the data and spit out the length angle and other sizes from the svg and i replicated it in Figma it was a huge pain to do it but i was able to and in not that long time And I'm really happy on how it turned out

![tree](https://cdn.hackclub.com/01a06d46-f787-7835-9ea4-7383b95abbe5/Screenshot_20260904_173524.png)

Then I converted it into a kicad silkscreen and made the PCB outline with the exact sizes of a credit card which i got from the wikipedia

![PCB](https://cdn.hackclub.com/01a06d49-fd36-744b-bd41-725a57e45a11/Screenshot_20260904_173810.png)

After that I added the Zones for the Touch Key which I saw how to do by Cyao on his Comu board and it goes on the right top corner the zones and this is where the Dot/Dash is done you just touch the pad and it registers

![touch key](https://cdn.hackclub.com/01a06d4e-1be4-7567-8262-6881ccd72440/Screenshot_20260904_174227.png)

**Total time spent: 3h**

# September 1: LEDs + Tree modifications

I assigned all Footprints, was pretty simple as I did not have many components. LEDs: 0402, Resistors: 0603, Capacitors 0603, Boot/Reset: SMD KMR2 buttons.

At first I was going to use the 0402 LEDs like this:

![led](https://cdn.hackclub.com/01a06d5e-4fef-7439-8134-82129c51a644/Screenshot_20260904_175139.png)

But then my sister had this really cool idea to make the LEDs shine thru the PCB and the LED's would sit on the other side of the PCB which would also make the front face flat which was nice. This all works in a really cool way firstly you take all the coper from a zone then take the mask then you are left with a FR4 surface and if you know that is kind of translucent well ligh can pass thru it which is really nice if you place and LED on the other side it acts as a diffuser and a few months ago I had seen something similar in this [Video](https://youtu.be/2100sdgQtL4?si=QfDrsLlIgj1CcjZS&t=279), after having this idea I researched how i could do this I first tought of just reversing a 0402 or something a bigger like a 1206 and yeah soldering it upside down and i tought nah that would be too janky so I researched a bit and found something called Reverse Gulwing LED which are slighly more expensive but produce the effect better and are more "proffesional" (After a while I confirmed that reverse soldering normal SMD LED's is something done in this [writeup](https://hackaday.io/page/6081-using-side-view-leds-in-place-of-reverse-mount-leds) Another reason for me not doing that is can't be assembled in PCBA)

After that I also tweaked the Tree a bit I made it slighly taller and narower like this I could make it bigger and the Letters would be more legible

![new tree](https://cdn.hackclub.com/01a06d77-12cf-7a21-b0be-8a4dcc6ee7d3/Screenshot_20260904_182820.png)

Then I had the actual tree size and I could start the custom footprint for the LED to incorporate the Soldermask taking so it would actually shine thru the PCB this was all really simple just used easyeda2kicad to convert the footprint then edit it in KiCAD with the Soldermask openning and added the 3D model.

And everything looks like this:

![footprint](https://cdn.hackclub.com/01a06d77-b454-733b-b2eb-903bc8cb6fbe/Screenshot_20260904_182414.png)
![PCB](https://cdn.hackclub.com/01a06d77-e382-749c-a917-2205b5de11ee/Screenshot_20260904_182628.png)
![PCB Back](https://cdn.hackclub.com/01a06d78-40d3-72c4-b1f5-90b1a05d3ec6/Screenshot_20260904_182944.png)

**Total time spent: 2h**

# September 2: Component placing

After I had the component footprints and everything I started placing everything first all the LEDs in place then the Battery holder which was really tight but still fit with very little space. 

This day i did very little but something i started was place all the LEDs and rotate them in the best way to route which was pretty easy to calculate which groups should go everywhere for the easiest routing which makes it very nice for routing.

I also added a new LED which is the Start LED which is the starting node and this is not in the Charlieplexing matrix because its just easier to add it to a sepparete GPIO

I also made the letters be copper so they have this cool look

![pcb](https://cdn.hackclub.com/01a06e7c-50e3-719e-9a91-f5b5498caa31/Screenshot_20260904_231304.png)
![3d](https://cdn.hackclub.com/01a06e82-489c-75a4-9ce8-1053c14bfbc9/Screenshot_20260904_231948.png)

**Total time spent: 1.5h**

# September 4: Routing + full component placement

Today I locked in and actually did most of the hard job routing everything.

I firstly routed all the LEDs which was a bit easier than I tought though its not the best routing ive done but as I'm slighly over due for submission i had to kinda rush it but should work.

When i got everything I started with the CH32 placement which i decided would go right near the battery which is where i have the most space and would be easy to route everything to there. I firstly placed the resistors for the Charlieplexing right near the output pins and that was easy to route every LED to then i moved on to the Reset / Boot buttons with Boot having the pull down and the debug pins right above everything I had basically everything routed when i rememeberd i needed the decoupling caps so i had to move a few components for them to fit but it was not that much of a change everything still fit really well.

![components](https://cdn.hackclub.com/01a06e94-cc04-734c-a84d-62680cc965eb/Screenshot_20260904_234018.png)

I then had to remake the dot/dash Zones to add a surface where i could add Vias so i could route the Zones

![zones](https://cdn.hackclub.com/01a06e96-71a6-7a50-8fa9-b8aaaf1d6deb/Screenshot_20260904_234156.png)

After this everything was basically done as the ch32 is so simple it requires almost nothing

The only thing that was missing was the GND pour and this was easy just draw around the PCB and setting the priority to 0 or then it would go on top of my Touch Key zones and we don't want that. And yeah thats it! I still had to fix a few GND traces so everything was connected but that was easy.

![full](https://cdn.hackclub.com/01a06e99-0085-7e72-a39b-e1e25b456a5f/Screenshot_20260904_234429.png)
![no zones visible](https://cdn.hackclub.com/01a06e99-fa09-7a7f-a4b0-c0ba8656858b/Screenshot_20260904_234612.png)

**Total time spent: 5h**

# September 4: repo, README, simple Firmware

The PCB was ready so i prepared everything to submit first i just wrote a simple README and generated a BOM with the JLC BOM I had.

Then I took screenshots and ploted the Schematic and added those to the README.

And finally the Firmware I found this simple example of various things the ch32 Touch Key and a simple morse code so I just pretty fast adopted a version its pretty simple and will update when i get the real thing in my hands.

![repo](https://cdn.hackclub.com/01a06f13-1a3b-7dde-b42d-3b7cb9ab7362/Screenshot%202026-09-05%20at%2001-57-28%20maker-lukas_morse-code-card.png)

**Total time spent: 2h**