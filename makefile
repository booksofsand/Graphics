Graphics: Graphics.o
	g++ -o Graphics Graphics.o
Graphics.o: Graphics.cc
	g++ -c Graphics.cc
clean:
	rm -f *~ *.o
