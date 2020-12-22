CXX = g++ 
CXXFLAGS = -w -Wall
LIBFLAGS =

SOURCE = util.cpp sop.cpp main.cpp

OBJECTS = util.o sop.o main.o

HEADERS = util.h

main: $(SOURCE)
	$(CXX) $(CXXFLAGS) -c $^
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(HEADERS) -o $@ $(LIBFLAGS)
	rm *.o

$(SOURCE) : $(HEADERS)

clean:
	rm main