#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <regex.h>
#include "document.h"
#include "regex.h"
#include "style.h"

#define GOOD_RETCODE 0
#define BAD_RETCODE 1

extern FILE * xmlin;
extern int xmlparse(Document **, std::ostream &);

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
  return BAD_RETCODE;
}

static Document * read_document(const char * fname)
{
  // Change xmlparse input
  FILE * fp = fopen(fname, "r");
  if (fp == nullptr)
  {
    std::cerr << "Unable to open file " << fname << std::endl;
    return nullptr;
  }
  xmlin = fp;

  // Construct document
  Document * doc = nullptr;
  std::ostringstream oss117;
  xmlparse(&doc, oss117); // who gives a f**k about the return code ?

  if (doc != nullptr)
  {
    doc->to_be_or_not_to_be(std::cerr);
  }
  if (!oss117.str().empty())
  {
    std::cerr << "syntax error" << std::endl << oss117.str();
    if (doc != nullptr)
    {
      delete doc;
      doc = nullptr;
    }
  }

  return doc;
}

int handle_parse(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide an argument to the command -p" << std::endl;
    return BAD_RETCODE;
  }

  // XML
  Document * doc = read_document(argv[0]);
  if (doc == nullptr) { return BAD_RETCODE; }
  std::cout << doc->str() << std::endl;
  delete doc;

  return GOOD_RETCODE;
}

int handle_validate(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide two arguments to the command -v: an xml file and an xsd file" << std::endl;
    return BAD_RETCODE;
  }
  else if (argc < 2)
  {
    std::cerr << "You must provide two arguments to the command -v: an xml file and an xsd file" << std::endl;
    return BAD_RETCODE;
  }

  // XML/XSD
  Document * xml_doc = read_document(argv[0]);
  if (xml_doc == nullptr)
  {
    return BAD_RETCODE;
  }

  Document * xsd_doc = read_document(argv[1]);
  if (xsd_doc == nullptr)
  {
    delete xml_doc;
    return BAD_RETCODE;
  }

  // Done
  std::string str_regex(xsd_to_regex(xsd_doc));
  regex_t regex;

  if (regcomp(&regex, str_regex.c_str(), REG_EXTENDED | REG_NOSUB) != 0)
  {
    delete xml_doc;
    delete xsd_doc;
    fprintf(stderr, "Could not compile regex\n");
    return BAD_RETCODE;
  }

  std::ifstream ifs(argv[0]);
  std::string xml_content((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));

  if (regexec(&regex, xml_content.c_str(), 0, NULL, 0) == 0)
  {
    std::cout << "The file " << argv[0] << " is valid wrt " << argv[1] << std::endl;
  }
  else
  {
    std::cout << "The file " << argv[0] << " is not valid wrt " << argv[1] << std::endl;
  }

  regfree(&regex);
  delete xml_doc;
  delete xsd_doc;

  return GOOD_RETCODE;
}

int handle_transform(int argc, const char ** argv)
{
  if (argc < 1)
  {
    std::cerr << "You must provide two arguments to the command -t: an xml file and an xsl file" << std::endl;
    return BAD_RETCODE;
  }
  else if (argc < 2)
  {
    std::cerr << "You must provide two arguments to the command -t: an xml file and an xsl file" << std::endl;
    return BAD_RETCODE;
  }

  // XML
  Document * xml_doc = read_document(argv[0]);
  if (xml_doc == nullptr)
  {
    return BAD_RETCODE;
  }

  // XML
  Document * xsl_doc = read_document(argv[1]);
  if (xsl_doc == nullptr)
  {
    delete xml_doc;
    return BAD_RETCODE;
  }

  xml_apply_style(*xml_doc, *xsl_doc, std::cout);

  delete xsl_doc;
  delete xml_doc;

  return GOOD_RETCODE;
}

// vim:ft=cpp et sw=2 sts=2:
