// importing required libraries
#include <cstddef>            // for system function
#include <cstdlib>            // for system function and getenv
#include <exception>          // exception handling
#include <fstream>            // file management
#include <getopt.h>           // for getting and managing commandline arguments
#include <iostream>           // printing out help various messages
#include <stdio.h>            // for getting and managing commandline arguments
#include <string>             // to work with strings
#include <tabulate/table.hpp> // for rendering fancy tables

// defining easier words to make life easier
#define str std::string
#define cout std::cout
#define sys std::system
#define read std::ifstream
#define getline std::getline
#define cin std::cin
#define write std::ofstream
#define to_string std::to_string
#define Table tabulate::Table
#define FontStyle tabulate::FontStyle
#define FontAlign tabulate::FontAlign
#define Color tabulate::Color
#define stoi std::stoi
#define getenv std::getenv
#define exception std::exception

// function to check whether text retrieved is empty or not
// and if empty then exit giving a message
void checkText(str txt) {
  if (txt == "") {
    cout << "no results found"
         << "\n";
    exit(1);
  }
}

// the function to do the manapage searching
void search_the_manual(str ask, int lines, str launcher, str searchTerm,
                       str path) {
  str txt;
  sys(("apropos " + str(ask) + " | " + launcher + " | cut -d ' ' -f1 > " +
       path + "temp.txt")
          .c_str());
  read temp(path + "temp.txt");
  getline(temp, txt);
  checkText(txt);
  sys(("man " + str(txt) + " > " + path + "temp.txt").c_str());
  int count = 1;
  if (searchTerm != "") {
    ask = searchTerm;
  }
  write temp2(path + "temp2.txt");
  while (getline(temp, txt)) {
    std::size_t found = txt.find(ask);
    if (found != str::npos) {
      temp2 << to_string(count) + txt + "\n";
    }
    count++;
  }
  temp2.close();
  sys(("cat " + path + "temp2.txt | " + launcher + " | cut -d ' ' -f1 > " +
       path + "temp3.txt")
          .c_str());
  read temp3(path + "temp3.txt");
  getline(temp3, txt);
  checkText(txt);
  int lineNum = stoi(txt);
  temp3.close();
  int startLine = lineNum - lines;
  int endLine = lineNum + lines;
  if (startLine < 0) {
    startLine = 0;
  }
  write temp4(path + "temp2.txt");
  count = 0;
  temp.close();
  read temp5(path + "temp.txt");
  while (getline(temp5, txt)) {
    if (count >= startLine && count <= endLine) {
      temp4 << txt + "\n";
    }
    count++;
  }
  temp4.close();
  str txt2 = "";
  read temp6(path + "temp2.txt");
  while (getline(temp6, txt)) {
    txt2 += txt + "\n";
  }

  // render the manpage excerpt in tabular format.
  Table table;
  table.add_row({"The following excerpt was taken from line " +
                 to_string(startLine) + " to " + to_string(endLine)});
  table.add_row({txt2});
  table.format()
      .padding(1)
      .font_style({FontStyle::bold})
      .border_top("—")
      .border_bottom("—")
      .border_left("│")
      .border_right("│")
      .corner("█");

  table[0]
      .format()
      .padding_top(1)
      .padding_bottom(1)
      .font_align(FontAlign::center)
      .font_style({FontStyle::bold})
      .font_color(Color::blue);

  cout << table;
  temp6.close();
}

// function to provide the whole manpage with example
void full_manual(str ask, str launcher, str path) {
  str txt;
  sys(("apropos " + str(ask) + " | " + launcher + " | cut -d ' ' -f1 > " +
       path + "temp.txt")
          .c_str());
  read temp(path + "temp.txt");
  read temp2(path + "temp2.txt");
  getline(temp, txt);
  checkText(txt);
  sys(("man " + str(txt) + " > " + path + "temp.txt").c_str());
  sys(("tldr " + str(txt) + " > " + path + "temp2.txt").c_str());
  str txt2;
  str txt3;
  while (getline(temp, txt)) {
    txt2 += txt + "\n";
  }
  while (getline(temp2, txt)) {
    txt3 += txt + "\n";
  }
  temp.close();
  temp2.close();

  // render the whole manual page with example in a tabular format
  Table table;
  table.add_row({"Abreviated --- Example Section"});
  table.add_row({txt3});
  table.add_row({"Elaborate --- Explanation Section"});
  table.add_row({txt2});

  table.format()
      .padding(1)
      .font_style({FontStyle::bold})
      .border_top("—")
      .border_bottom("—")
      .border_left("│")
      .border_right("│")
      .corner("█");

  table[0]
      .format()
      .padding_top(1)
      .padding_bottom(1)
      .font_align(FontAlign::center)
      .font_style({FontStyle::bold})
      .font_color(Color::blue);

  table[2]
      .format()
      .padding_top(1)
      .padding_bottom(1)
      .font_align(FontAlign::center)
      .font_style({FontStyle::bold})
      .font_color(Color::blue);

  cout << table;
}

// function to give only the example manual
void example_manual(str ask, str launcher, str path) {
  str txt;
  sys(("apropos " + str(ask) + " | " + launcher + " | cut -d ' ' -f1 > " +
       path + "temp.txt")
          .c_str());
  read temp(path + "temp.txt");
  getline(temp, txt);
  checkText(txt);
  sys(("tldr " + str(txt) + " > " + path + "temp.txt").c_str());
  str txt2;
  while (getline(temp, txt)) {
    txt2 += txt + "\n";
  }
  temp.close();

  // render the example manual page in a tabular format
  Table table;
  table.add_row({"Example Section"});
  table.add_row({txt2});

  table.format()
      .padding(1)
      .font_style({FontStyle::bold})
      .border_top("—")
      .border_bottom("—")
      .border_left("│")
      .border_right("│")
      .corner("█");

  table[0]
      .format()
      .padding_top(1)
      .padding_bottom(1)
      .font_align(FontAlign::center)
      .font_style({FontStyle::bold})
      .font_color(Color::blue);

  cout << table;
}

// function to do commandline argument checking
// -- it checks whether the argument provide to option "-l"
// -- is a number or not
// -- if not then prints an error message and quits out
int checklines(str lines) {
  try {
    return stoi(lines);
  } catch (exception) {
    cout << "invalid value for argument -l"
         << "\n";
    exit(1);
  }
}

// function to print out usage information on the option "-h"
void usage() {
  cout << "Usage: "
       << "stdm"
       << " [options] "
       << "<name>"
       << "\n";
  cout << "\n";
  cout << "\t-h"
       << "\tprovide this help list"
       << "\n";
  cout << "\t-f"
       << "\tdisplay the full manual with example manual if available"
       << "\n";
  cout << "\t-s"
       << "\tsearch the manual page with the functionality"
       << "\n";
  cout << "\t-e"
       << "\tgive the example manual if available"
       << "\n";
  cout << "\t-l"
       << "\tthe number of lines of excerpt to display from the manual "
          "(default: 5)"
       << "\n";
  cout << "\t-L"
       << "\tprovide an appropriate launcher (default: fzf)"
       << "\n";
  cout << "\t-S"
       << "\tsearch the manual by providing a search term should be used with "
          "option '-s' (default: None)"
       << "\n";
  cout << "\n";
  cout << "Report bugs @ https://gitlab.com/NEON-MMD/stdm/-/issues";
}

// --------------------------------- main
// ----------------------------------------
int main(int argc, char *argv[]) {
  // get the home path.
  str const HOME = getenv("HOME") ? getenv("HOME") : ".";

  // get the cache path.
  str const PATH = HOME + "/.cache/";

  // default number of lines of excerpt to be printed from the manpage
  // if option "-l" is not provided.
  int lines = 5;

  // default launcher if option "-L" is not provided.
  str launcher = "fzf";

  // default search term if option "-S" is not provided.
  str searchTerm = "";

  // define all options and do stuff accordingly
  int opt;
  while ((opt = getopt(argc, argv, "he:s:f:l:L:S:")) != -1) {
    switch (opt) {
    case 'L':
      launcher = str(optarg);
      break;
    case 'l':
      lines = checklines(optarg);
      break;
    case 's':
      search_the_manual(str(optarg), lines, launcher, searchTerm, PATH);
      break;
    case 'S':
      searchTerm = str(optarg);
      break;
    case 'f':
      full_manual(str(optarg), launcher, PATH);
      break;
    case 'e':
      example_manual(str(optarg), launcher, PATH);
      break;
    case 'h':
      usage();
      break;
    case '?':
      cout << "stdm: invalid option"
           << "\n";
      cout << "Try stdm -h for more information"
           << "\n";
      break;
    }
  }
  return 0;
}
