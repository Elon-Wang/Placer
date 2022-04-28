gcc -O2 -c graphics.c
c++ -O2 -c Cell.cpp
c++ -O2 -c Wire.cpp
c++ -O2 -c -Wno-unused-result Placer.cpp
c++ -O2 -c -Wno-unused-result main.cpp
c++ -O2 main.o graphics.o Cell.o Wire.o Placer.o -o Placer -L/usr/openWin/lib -lX11 -lm
rm *.o

echo "finish compiling, try Placer now"
echo ""
echo "Usage: ./Placer <benchmark-name> [--detail <1-3>]"
echo "example: ./Placer cm150a --detail 2"