CC    ?= gcc
CXX   ?= g++

EXE = bin/kompilator

CDEBUG = -g -Wall
CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -O0  $(CDEBUG) $(CSTD) 
CXXFLAGS = -O0  $(CXXDEBUG) $(CXXSTD)

DIRS = obj bin

OBJPATH = obj/
FRONTPATH = src/front/
BACKPATH = src/back/

FBELEMENTS =  parser lexer

FRONTELEMENTS = driver main writer
FRONTFILES = $(addsuffix .cpp, $(addprefix src/front/, $(FRONTELEMENTS)))

HANDLINGELEMENTS = symbol memory_manager symbol_table node expression assignment label condition branch branch_else while_do do_while for_to for_down_to write read code_block handler 
HANDLINGFILES = $(addsuffix .cpp, $(addprefix src/handling/, $(HANDLINGELEMETS)))

BACKELEMENTS = optimizer
BACKFILES = $(addsuffix .cpp, $(addprefix src/back/, $(BACKELEMENTS)))

OBJS  = $(addprefix $(OBJPATH), $(addsuffix .o, $(FBELEMENTS) $(FRONTELEMENTS) $(BACKELEMENTS) $(HANDLINGELEMENTS)))


all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)

obj/%.o: src/front/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/handling/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/back/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/parser.o: $(FRONTPATH)parser.tab.cc
	$(CXX) $(CXXFLAGS) -c -o $(OBJPATH)parser.o $(FRONTPATH)parser.tab.cc

$(FRONTPATH)parser.tab.cc : $(FRONTPATH)parser.yy
	bison -d -v -o $(FRONTPATH)parser.tab.cc $(FRONTPATH)parser.yy

obj/lexer.o: $(FRONTPATH)lexer.yy.cc
	$(CXX)  $(CXXFLAGS) -c $(FRONTPATH)lexer.yy.cc -o $(OBJPATH)lexer.o
	
$(FRONTPATH)lexer.yy.cc: $(FRONTPATH)lexer.l
	flex --outfile=$@ $<
	



