CXX = g++
WARNFLAGS = -W -Wall
CXXFLAGS = -std=c++0x $(WARNFLAGS)
LD=g++
LDFLAGS=

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		composite_element.cpp content.cpp document.cpp element.cpp \
		empty_element.cpp main.cpp prolog.cpp
HEADERS = $(subst .cpp,.h,$(SOURCES))
OBJECTS = $(subst .cpp,.o,$(SOURCES))
EXE = xmltool

all: $(EXE)
	
$(EXE): grammar objects
	g++ -std=c++11 -g -DYYDEBUG -o $(EXE) $(OBJECTS) xml.tab.c lex.xml.c

grammar: *.l *.y
	flex -P xml xml.l
	bison -p xml --debug --verbose --defines=xml.tab.h xml.y

objects: $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -rf xml.tab.c lex.xml.c xml.tab.h xml.output $(EXE)

test:
	cd Tests; ./mktest.sh

# vim:ft=make noet sw=4 ts=4:
