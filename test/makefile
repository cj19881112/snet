CC=clang++

all : clean Discard.out Echo.out

Discard.out : Discard.cpp ../*.cpp
	$(CC) -Wall -o $@ $^  -I.. -g

Echo.out : EchoServer.cpp ../*.cpp
	$(CC) -Wall -o $@ $^  -I.. -g

clean : 
	rm -rf Discard.out  Echo.out
