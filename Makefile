CXX = g++
WARNFLAGS = -W -Wall
CXXFLAGS = -std=c++0x $(WARNFLAGS)

SOURCES = $(wildcard *.cpp)
SOURCES = main.cpp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

GRAMMAR = commun
EXE = xmltool

.PHONY: all clean test

all: $(EXE)
	
$(EXE): $(GRAMMAR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -g -DYYDEBUG -o $(EXE) $(OBJECTS) xml.tab.c lex.xml.c

$(GRAMMAR): xml.l xml.y
	flex -P xml xml.l
	bison -p xml --debug --verbose --defines=xml.tab.h xml.y
	touch $(GRAMMAR)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# TODO dépendances supplémentaires

clean:
	@rm -f $(OBJECTS) $(GRAMMAR) $(EXE)
	@rm -rf xml.tab.c lex.xml.c xml.tab.h xml.output

test:
	cd Tests; ./mktest.sh

# vim:ft=make noet sw=4 ts=4:
