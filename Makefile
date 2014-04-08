CXX = g++
WARNFLAGS = -W -Wall
CXXFLAGS = -std=c++0x $(WARNFLAGS)
LD=g++
LDFLAGS=

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		composite_element.cpp content.cpp document.cpp element.cpp \
		empty_element.cpp main.cpp prolog.cpp
OBJECTS = $(subst .cpp,.o,$(SOURCES))
GRAMMAR = commun
EXE = xmltool

.PHONY: all objects clean test

all: $(EXE)
	
$(EXE): objects $(GRAMMAR)
	g++ -std=c++11 -g -DYYDEBUG -o $(EXE) $(OBJECTS) xml.tab.c lex.xml.c

$(GRAMMAR): xml.l xml.y
	flex -P xml xml.l
	bison -p xml --debug --verbose --defines=xml.tab.h xml.y
	touch $(GRAMMAR)

objects: $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# TODO dépendances supplémentaires

clean:
	@rm -f $(OBJECTS) $(GRAMMAR) $(EXE)
	@rm -rf xml.tab.c lex.xml.c xml.tab.h xml.output

test:
	cd Tests; ./mktest.sh

# vim:ft=make noet sw=4 ts=4:
