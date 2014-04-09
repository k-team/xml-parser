CXX = g++
WARNFLAGS = -W -Wall -Wextra -Wfloat-equal -Wundef -Wpointer-arith \
			-Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual \
			-Wswitch-enum -Wunreachable-code
CXXFLAGS = -pedantic -std=c++0x -g $(WARNFLAGS)

LD = g++
LDFLAGS =

SOURCES = attribute.cpp base.cpp cd_sect.cpp char_data.cpp \
		  composite_element.cpp content.cpp doctype.cpp document.cpp \
		  element.cpp main.cpp pi.cpp prolog.cpp xml_exception.cpp \
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
xml.l: content.h doctype.h attribute.h document.h
xml.y: attribute.h cd_sect.h char_data.h composite_element.h content.h \
	doctype.h document.h element.h pi.h prolog.h
attribute.h: base.h
cd_sect.h: content.h
char_data.h: content.h
composite_element.h: element.h
composite_element.cpp: content.h attribute.h
content.h: base.h
doctype.h: base.h
document.h: base.h
document.cpp: prolog.h element.h pi.h
element.h: content.h
element.cpp: attribute.h
main.cpp: document.h
pi.h: content.h
pi.cpp: attribute.h
prolog.h: base.h
prolog.cpp: doctype.h pi.h

test: all
	cd Tests; ./mktest.sh

clean:
	@rm -f $(OBJECTS) $(EXE)
	@rm -f xml.tab.cpp xml.tab.h
	@rm -f lex.xml.cpp xml.output

# vim:ft=make noet sw=4 ts=4:
