CC    ?= gcc
CXX   ?= g++

EXE = bin/kompilator

CDEBUG = -g -Wall
CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0  $(CDEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD)

DIRS = obj bin

OBJPATH = obj/
FRONTPATH = src/front/
BACKPATH = src/back/

FBELEMENTS =  parser lexer

FRONTELEMENTS = driver main
FRONTFILES = $(addsuffix .cpp, $(addprefix src/front/, $(FRONTELEMENTS)))

BACKELEMENTS = compiler
BACKFILES = $(addsuffix .cpp, $(addprefix src/back/, $(BACKELEMENTS)))

OBJS  = $(addprefix $(OBJPATH), $(addsuffix .o, $(FBELEMENTS) $(FRONTELEMENTS) $(BACKELEMENTS)))


all:
	$(info $(DIRS))
	$(info $(shell mkdir -p $(DIRS)))
	$(MAKE) $(FBELEMENTS)
	$(MAKE) $(FRONTELEMENTS)
	$(MAKE) $(BACKELEMENTS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LIBS)


parser: $(FRONTPATH)parser.yy
	bison -d -v -o $(FRONTPATH)parser.tab.cc $(FRONTPATH)parser.yy
	$(CXX) $(CXXFLAGS) -c -o $(OBJPATH)parser.o $(FRONTPATH)parser.tab.cc

lexer: $(FRONTPATH)lexer.l
	flex --outfile=$(FRONTPATH)lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c $(FRONTPATH)lexer.yy.cc -o $(OBJPATH)lexer.o

driver: parser lexer
	$(CXX)  $(CXXFLAGS) -c -o $(OBJPATH)driver.o $(BACKFILES)

compiler: driver
	$(CXX)  $(CXXFLAGS) -c -o $(OBJPATH)compiler.o $(FRONTPATH)driver.cpp

main:
	$(CXX)  $(CXXFLAGS) -c -o $(OBJPATH)main.o $(FRONTPATH)main.cpp





