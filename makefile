graphics: graphics.o
	g++ -o graphics graphics.o
graphics.o: graphics.cc
	g++ -c graphics.cc
clean:
	rm -f *~ *.o tester
