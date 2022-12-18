#ifndef PROJECT5_EXCEPTION_H
#define PROJECT5_EXCEPTION_H

#include <cstring>
#include <exception>
#include <iostream>

// 异常类
class Exception : public std::exception {
 private:
  std::string Exception_details;
  std::string fileName;
  int lineNumber;

 public:
  explicit Exception(std::string Exception_details = "default error",
                     std::string fileName = __FILE__,
                     int lineNumber = __LINE__) {
    this->Exception_details = std::move(Exception_details);
    this->fileName = std::move(fileName);
    this->lineNumber = lineNumber;
  }

  void errorHandle() {
    std::cerr << Exception_details << "at file:" << fileName
              << ", line: " << lineNumber << "\n";
  }
};

#endif
