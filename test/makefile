CC=clang++

TARGET=Discard.out EchoSer.out

all : clean $(TARGET)

Discard.out : Discard.cpp ../*.cpp
	$(CC) -Wall -o $@ $^  -I.. -g

EchoSer.out : EchoServer.cpp ../*.cpp
	$(CC) -Wall -o $@ $^  -I.. -g


clean : 
	rm -rf $(TARGET)
