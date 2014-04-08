#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "document.h"

extern FILE * xmlin;
extern int xmlparse(Document **);

static std::string help_message = "Available commands are:\n\
../xmltool -p file.xml : parse and display the xml file\n\
../xmltool -v file.xml file.xsd : parse both xml and xsd files and display the validation result\n\
../xmltool -t file.xml file.xsl : parse both xml and xsl files and display de transformation result of file.xml by the stylesheet file.xsl\n\
../xmltool -h : displays this help\n";

static int handle_help(int, const char **);
static int handle_validate(int, const char **);
static int handle_parse(int, const char **);
static int handle_transform(int, const char **);

// Valid commands (no dash character)
typedef int (*command_handler)(int, const char **) ;
static std::map<std::string, command_handler> command_map = {
  { "h", handle_help},
  { "v", handle_validate },
  { "p", handle_parse },
  { "t", handle_transform }
};

int main(int argc, const char ** argv)
{
  // Command must be given
  if (argc < 2)
  {
    std::cerr << "No argument given" << std::endl;
    return handle_help(argc, argv);
  }

  // Command must be handled
  std::string cmd(argv[1] + 1);
  std::map<std::string, command_handler>::const_iterator it;
  if ((it = command_map.find(cmd)) == command_map.end())
  {
    std::cerr << "Bad argument given" << std::endl;
    return handle_help(argc, argv);
  }

  // Call command handler, making argc and argv correspond
  int nb_ignored = 2;
  argc -= nb_ignored;
  argv += nb_ignored;
  return it->second(argc, argv);
}

int handle_help(int, const char **)
{
  std::cerr << help_message;
  return 1;
}

static Document * read_document(const char * fname)
{
  // Change xmlparse input
  FILE * fp = fopen(fname, "r");
  if (fp == nullptr)
  {
    return nullptr;
  }
  xmlin = fp;

  // Construct document
  Document * doc = nullptr;
  xmlparse(&doc); // who gives a f**k about the return code ?

  // TODO semantic analysis

  return doc;
}

int handle_parse(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide an argument to the command -p" << std::endl;
    return EXIT_FAILURE;
  }

  // XML
  Document * doc = read_document(argv[0]);
  if (doc == nullptr) { return EXIT_FAILURE; }
  std::cout << doc->str() << std::endl;
  delete doc;

  return EXIT_SUCCESS;
}

int handle_validate(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide two arguments to the command -v: an xml file and an xsd file" << std::endl;
    return EXIT_FAILURE;
  }
  else if (argc < 2)
  {
    std::cerr << "You must provide two arguments to the command -v: an xml file and an xsd file" << std::endl;
    return EXIT_FAILURE;
  }

  // XML/XSD
  Document * xml_doc = read_document(argv[0]);
  Document * xsd_doc = read_document(argv[1]);

  // TODO

  delete xml_doc;
  delete xsd_doc;

  return EXIT_SUCCESS;
}

int handle_transform(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide two arguments to the command -t: an xml file and an xsl file" << std::endl;
    return EXIT_FAILURE;
  }
  else if (argc < 2)
  {
    std::cerr << "You must provide two arguments to the command -t: an xml file and an xsl file" << std::endl;
    return EXIT_FAILURE;
  }

  // XML/XSL
  Document * xml_doc = read_document(argv[0]);
  Document * xsl_doc = read_document(argv[1]);

  // TODO

  delete xsl_doc;
  delete xml_doc;

  return EXIT_FAILURE;
}

// vim:ft=cpp et sw=2 sts=2:
