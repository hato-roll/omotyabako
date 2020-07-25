CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(GTEST_INCLUDEDIR) -L$(GTEST_LIBDIR) -lgtest -lgtest_main -lpthread
OBJS = test

all: 
	$(CXX) $(OBJS).cc -o $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(OBJS)
