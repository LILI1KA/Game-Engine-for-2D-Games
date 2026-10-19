#ifndef STATUS_H
#define STATUS_H
#include <ncurses.h>
#include <string>

class Status {
    std::string line_1;
    std::string line_2;
    std::string line_3;
  public:
    Status();
    void get_line(std::string &str1, std::string &str2, std::string &str3);
    void set_line(std::string str1, std::string str2, std::string str3);
};


#endif
