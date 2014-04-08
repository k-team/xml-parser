CXX = g++
WARNFLAGS = -W -Wall
CXXFLAGS = -pedantic -std=c++0x -g $(WARNFLAGS)

LD = g++
LDFLAGS =

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		  composite_element.cpp content.cpp doctype.cpp document.cpp \
		  element.cpp main.cpp pi.cpp prolog.cpp xml_exception.cpp \
		  xml.tab.cpp lex.xml.cpp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

GRAMMAR = commun
EXE = xmltool

.PHONY: all clean test

all: $(EXE)
	
$(EXE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -DYYDEBUG -o $(EXE)

lex.xml.cpp: xml.l
	flex -o lex.xml.cpp -P xml xml.l

xml.tab.cpp: xml.y
	bison -o xml.tab.cpp -p xml --debug --verbose --defines=xml.tab.h xml.y

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cpp: %.h

# TODO dépendances supplémentaires

test: all
	cd Tests; ./mktest.sh

clean:
	@rm -f $(OBJECTS) $(GRAMMAR) $(EXE)
	@rm -f xml.tab.cpp xml.tab.h
	@rm -f lex.xml.cpp xml.output

# vim:ft=make noet sw=4 ts=4:
