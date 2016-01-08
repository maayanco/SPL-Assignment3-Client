CFLAGS:=-c -Wall -g
LDFLAGS:=-lboost_system -lboost_locale 

MDFLAGS:= -L"C:\installation\path\lib" -lboost_locale
HDFLAGS:= -L"C:\installation\path\lib" -lboost_system
EDFLAGS:= -L"C:\installation\path\lib" -lboost_thread

all: bin/encoder.o EchoClient
	g++ -o bin/echoExample bin/encoder.o bin/connectionHandler.o bin/echoClient.o $(MDFLAGS) $(HDFLAGS) $(EDFLAGS)

EchoClient: bin/connectionHandler.o bin/echoClient.o
	
bin/connectionHandler.o: echoClient/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o echoClient/connectionHandler.cpp

bin/echoClient.o: echoClient/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o echoClient/echoClient.cpp
	
bin/encoder.o: encoder/encoder.cpp
	g++ $(CFLAGS) -o bin/encoder.o encoder/encoder.cpp $(MDFLAGS) $(HDFLAGS) $(EDFLAGS)
	
.PHONY: clean
clean:
	rm -f bin/*
