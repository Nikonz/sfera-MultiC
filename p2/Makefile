SRC = chat.cpp chatsrv.cpp
HDR = chat.h

all: chatsrv

chatsrv: $(SRC) $(HDR)
	g++ -g -std=c++11 -o chatsrv $(HDR) $(SRC)

clean:
	rm -f chatsrv

