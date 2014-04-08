#include <algorithm>
#include <map>
#include <iostream>

extern int xmlparse(void);

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
      << help_message << std::endl;
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

  // Call command handler
  return it->second(argc, argv);
}

int handle_help(int argc, const char ** argv)
{
  std::cout << help_message << std::endl;
  return 0;
}

int handle_validate(int argc, const char ** argv)
{
  // TODO
  return 1;
}

int handle_process(int argc, const char ** argv)
{
  // TODO
  return 1;
}

int handle_transform(int argc, const char ** argv)
{
  // TODO
  return 1;
}

// vim:ft=cpp et sw=2 sts=2:
