# Timber-Man-Game
Timber Man is a classic 2D arcade In this game, players take on the role of a lumberjack tasked with chopping down a towering tree while avoiding branches that fall from above. The objective is simple: chop down as many branches as possible without getting hit, and see how high of a score you can achieve!

# How to Run Timber Man Game🌲

## Running the Executable

Simply run the `app.exe` file to start playing Timber Man.

## Running from Source (Window)

If you prefer to compile and run from source, follow these steps:

1. **Compile**:

    ```bash
    g++ -IC:\SFML-2.6.1\include -c timber.cpp -o timber.o
    ```

2. **Link**:

    ```bash
    g++ -LC:/SFML-2.5.1/lib .\timber.o -o app.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows
    ```

These commands compile and link the source code to create the executable.

Now you're ready to chop some wood! 🪓🌳
