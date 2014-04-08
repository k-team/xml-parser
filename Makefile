CXX = g++
WARNFLAGS = -W -Wall
CXXFLAGS = -std=c++0x $(WARNFLAGS)

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		  composite_element.cpp content.cpp doctype.cpp document.cpp \
		  element.cpp main.cpp pi.cpp prolog.cpp xml_exception.cpp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

GRAMMAR = commun
EXE = xmltool

.PHONY: all clean test

all: $(EXE)
	
$(EXE): $(GRAMMAR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -g -DYYDEBUG -o $(EXE) $(OBJECTS) xml.tab.o lex.xml.o

$(GRAMMAR): xml.l xml.y
	flex -o lex.xml.cpp -P xml xml.l
	bison -o xml.tab.cpp -p xml --debug --verbose --defines=xml.tab.h xml.y
	$(CXX) $(CXXFLAGS) -c xml.tab.cpp -o xml.tab.o
	$(CXX) $(CXXFLAGS) -c lex.xml.cpp -o lex.xml.o
	@touch $(GRAMMAR)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# TODO dépendances supplémentaires

clean:
	@rm -f $(OBJECTS) $(GRAMMAR) $(EXE)
	@rm -rf xml.tab.cpp lex.xml.cpp xml.tab.h xml.output

test:
	cd Tests; ./mktest.sh

# vim:ft=make noet sw=4 ts=4:
