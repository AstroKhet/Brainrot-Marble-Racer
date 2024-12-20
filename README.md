# Brainrot Marble Racer

Characters from the popular show *SpongeBob SquarePants* race against each other to the finish line but are hindered by immovable obstacles. Who will be the first to reach the finish line?

Made with C++ and [SFML](https://www.sfml-dev.org/) 

AI voices produced with [Singify](https://singify.fineshare.com/)

Current song being used in `audio/` is [KSI - Thick Of It](https://youtu.be/At8v_Yc044Y?si=ClZ-Aq1LY8vtuVgQ)

## Features  
- Play as popular *Spongebob Squarepants* characters.
- Dynamic race environment with obstacles that each character must navigate.
- Create your own obstacle courses 
- Exciting race mechanics to simulate a true race-to-the-finish atmosphere.


## How to install
- Clone this repository.
- Install SFML and set up your folder. Move all files here.

## How to execute
- Use the command `make` to create the executable `main.exe`
- Run the executable with `./main`

## Customize race map
All the map information is stored within `map.txt`. Between each newline, you should write the *x* and *y* values of each vertice in a polygon. Comments can be added with `#` for reference.

## Screengrabs
![Start](pictures/readme/start.png)
![Racing](pictures/readme/racing.png)
![Finish](pictures/readme/win.png)
