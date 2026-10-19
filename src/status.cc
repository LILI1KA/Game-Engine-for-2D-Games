#include <ncurses.h>
#include <string>
#include "status.h"

Status::Status(): line_1{""}, line_2{""}, line_3{""} {}

void Status::get_line(std::string &str1, std::string &str2, std::string &str3) {
    str1 = line_1;
    str2 = line_2;
    str3 = line_3;
}
    
void Status::set_line(std::string str1, std::string str2, std::string str3) {
    line_1 = str1;
    line_2 = str2;
    line_3 = str3;
}
