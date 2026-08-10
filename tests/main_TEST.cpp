#include <arpa/inet.h>

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

#include "file_logger.hpp"
#include "socket_logger.hpp"

namespace testing {

class Tester {
 public:
  virtual void test() = 0;
  virtual ~Tester() = default;
};

class FlTester : public Tester {
 public:
  void test() override;
};

class SlTester : public Tester {
 public:
  void test() override;
};

}  // namespace testing

namespace testing {

void FlTester::test() {
  const std::string test_filename = "test_fl.log";
  logging::ErrorCode ec;

  ec = fl_api::init(test_filename, logging::LogLevel::DEBUG);
  assert(ec == 0);

  ec = fl_api::log("debug msg", logging::LogLevel::DEBUG);
  assert(ec == 0);
  ec = fl_api::log("info msg", logging::LogLevel::INFO);
  assert(ec == 0);
  ec = fl_api::log("error msg", logging::LogLevel::ERROR);
  assert(ec == 0);

  fl_api::setLevel(logging::LogLevel::INFO);

  ec = fl_api::log("debug ignored", logging::LogLevel::DEBUG);
  assert(ec == 0);

  ec = fl_api::log("info after change", logging::LogLevel::INFO);
  assert(ec == 0);
  ec = fl_api::log("error after change", logging::LogLevel::ERROR);
  assert(ec == 0);

  fl_api::close();

  std::ifstream file(test_filename);
  assert(file.is_open());

  std::string line;
  bool found_debug = false;
  bool found_info = false;
  bool found_error = false;
  bool found_debug_ignored = false;
  bool found_info_after = false;
  bool found_error_after = false;

  while (std::getline(file, line)) {
    if (line.find("debug msg") != std::string::npos) found_debug = true;
    if (line.find("info msg") != std::string::npos) found_info = true;
    if (line.find("error msg") != std::string::npos) found_error = true;
    if (line.find("debug ignored") != std::string::npos)
      found_debug_ignored = true;
    if (line.find("info after change") != std::string::npos)
      found_info_after = true;
    if (line.find("error after change") != std::string::npos)
      found_error_after = true;
  }
  file.close();

  assert(found_debug);
  assert(found_info);
  assert(found_error);
  assert(!found_debug_ignored);
  assert(found_info_after);
  assert(found_error_after);

  std::remove(test_filename.c_str());

  std::cout << "FlTester: ALL TESTS PASSED\n";
}

void SlTester::test() {
  logging::ErrorCode ec;

  ec = sl_api::log("message without init", logging::LogLevel::DEBUG);
  assert(ec == 1);

  uint32_t ip = inet_addr("127.0.0.1");
  uint16_t port = 9999;
  ec = sl_api::init(ip, port, logging::LogLevel::DEBUG);
  assert(ec == 5);

  ec = sl_api::log("message after failed init", logging::LogLevel::DEBUG);
  assert(ec == 1);

  std::cout << "SlTester: ALL TESTS PASSED\n";
}

}  // namespace testing

int main() {
  std::cout << "=== Running tests ===\n";

  testing::FlTester fl_test;
  fl_test.test();

  testing::SlTester sl_test;
  sl_test.test();

  std::cout << "=== All tests passed successfully ===\n";
  return 0;
}