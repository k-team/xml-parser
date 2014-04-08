#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "document.h"

extern FILE * xmlin;
extern int xmlparse(Document **);

static std::string help_message = \
"                                                                             \n\
Usage:                                                                        \n\
                                                                              \n\
   xmltool OPTION [ARGS...]                                                   \n\
                                                                              \n\
Options:                                                                      \n\
   -h                      Display this help                                  \n\
   -v file.xml             Validate XML file's syntax.                        \n\
   -p file.xml             Process XML file and display on standard output.   \n\
   -t file.xml file.xsl    Apply stylesheet to XML file.                      \n\
";

static int handle_help(int, const char **);
static int handle_validate(int, const char **);
static int handle_process(int, const char **);
static int handle_transform(int, const char **);

// Valid commands (no dash character)
typedef int (*command_handler)(int, const char **) ;
static std::map<std::string, command_handler> command_map = {
  { "h", handle_help},
  { "v", handle_validate },
  { "p", handle_process },
  { "t", handle_transform }
};

int main(int argc, const char ** argv)
{
  // Command must be given
  if (argc < 2)
  {
    std::cerr << "Missing option parameter!" << std::endl
      //<< help_message << std::endl
    ;
    return EXIT_FAILURE;
  }

  // Command must be handled
  std::string cmd(argv[1] + 1);
  std::map<std::string, command_handler>::const_iterator it;
  if ((it = command_map.find(cmd)) == command_map.end())
  {
    std::cerr << "Bad option parameter!" << std::endl
      << help_message << std::endl;
    return EXIT_FAILURE;
  }

  // Call command handler, making argc and argv correspond
  int nb_ignored = 2;
  argc -= nb_ignored;
  argv += nb_ignored;
  return it->second(argc, argv);
}

int handle_help(int, const char **)
{
  std::cout << help_message << std::endl;
  return 0;
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
  int re = xmlparse(&doc); // who gives a f**k about the return code ?
  if (doc != nullptr)
  {
    std::cout << "Entrée standard reconnue" << std::endl;
  }
  else
    std::cout << "Entrée standard non reconnue" << std::endl;
  return doc;
}

int handle_validate(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "Filename argument required" << std::endl;
    return EXIT_FAILURE;
  }

  // Open and handle errors
  Document * doc = read_document(argv[0]);
  if (doc == nullptr)
  {
    // TODO error message ?
    return EXIT_FAILURE;
  }
  delete doc;

  return EXIT_FAILURE;
}

int handle_process(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "Filename argument required" << std::endl;
    return EXIT_FAILURE;
  }

  // Open and handle errors
  Document * doc = read_document(argv[0]);
  if (doc == nullptr)
  {
    // TODO error message ?
    return EXIT_FAILURE;
  }

  std::cout << doc->str() << std::endl;
  delete doc;

  return EXIT_FAILURE;
}

int handle_transform(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "XML filename argument required" << std::endl;
    return EXIT_FAILURE;
  }
  else if (argc < 2)
  {
    std::cerr << "XSL filename argument required" << std::endl;
    return EXIT_FAILURE;
  }

  // Open and handle errors
  Document * xml_doc = read_document(argv[0]);
  // TODO
  delete xml_doc;

  // Open and handle errors
  Document * xsl_doc = read_document(argv[1]);
  // TODO
  delete xsl_doc;

  return EXIT_FAILURE;
}

// vim:ft=cpp et sw=2 sts=2:
