CXX = g++
WARNFLAGS = -W -Wall -Wextra -Wfloat-equal -Wundef -Wpointer-arith \
			-Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual \
			-Wswitch-enum -Wunreachable-code
CXXFLAGS = -pedantic -std=c++0x -g $(WARNFLAGS)

LD = g++
LDFLAGS =

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		  composite_element.cpp content.cpp doctype.cpp document.cpp \
		  element.cpp main.cpp pi.cpp prolog.cpp regex.cpp \
		  xml.tab.cpp lex.xml.cpp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

EXE = xmltool

.PHONY: all clean test

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -DYYDEBUG -o $(EXE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cpp: %.h

# These two are generated
lex.xml.cpp: xml.l
	flex -o lex.xml.cpp -P xml xml.l

xml.tab.cpp: xml.y
	bison -o xml.tab.cpp -p xml --debug --verbose --defines=xml.tab.h xml.y

# Specified dependencies
lex.xml.o: content.h doctype.h attribute.h document.h
xml.tab.o: attribute.h cd_sect.h char_data.h composite_element.h content.h \
	doctype.h document.h element.h pi.h prolog.h
attribute.o: base.h
cd_sect.o: base.h content.h
char_data.o: content.h
composite_element.o: element.h attribute.h
content.o: base.h
doctype.o: base.h
document.o: base.h prolog.h element.h pi.h
element.o: content.h attribute.h
main.o: document.h
pi.o: content.h attribute.h
prolog.o: base.h doctype.h pi.h

test: all
	cd Tests; ./mktest.sh

clean:
	@rm -f $(OBJECTS) $(EXE)
	@rm -f xml.tab.cpp xml.tab.h
	@rm -f lex.xml.cpp xml.output

# vim:ft=make noet sw=4 ts=4:
