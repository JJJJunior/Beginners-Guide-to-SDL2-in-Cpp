g++ -Isrc/include -Lsrc/lib -o 01-open-window 01-open-window.cpp -lmingw32 -lSDL2main -lSDL2 -std=c++20

g++ -Isrc/include -Lsrc/lib -o 03-background 03-background.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -std=c++20

g++ -Isrc/include -Lsrc/lib -o 05-creating-text 05-creating-text.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -std=c++20