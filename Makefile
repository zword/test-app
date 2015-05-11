#!/bin/bash

TARGET = server

CXXFLAGS=-c -Wall -Wextra -g -std=c++11 -Wno-literal-suffix -pthread

SRC_CPP = $(wildcard *.cpp)
SRC_H = $(wildcard *.hpp)

OBJS=$(SRC_CPP:.cpp=.o)

LDFLAGS= -pthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp $(SRC_H)
#	@echo INC=[${INC}]
#	$(CXX) -I../include $(INC) $(CXXFLAGS) $< -o $@
	$(CXX) ${INC} $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) *~

