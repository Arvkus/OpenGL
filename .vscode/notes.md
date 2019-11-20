

```sh
# g++ -Wall -std=c++17 cpp/src/*.cpp -o &quot;cpp/bin/main.out&quot; -I cpp/include -lGLESv2
# -lSDL2 -lassimp
g++ -std=c++17 src/* -o bin/main.exe -I include
g++ -Wall -std=c++17 src/* -o bin/main.exe -I include -L lib -lSDL2
g++ -std=c++17 src/* -o bin/main.exe -I include -L lib

g++ -Wall -std=c++17 src/* -o bin/main.exe -I include -L lib -lSDL2
 
```