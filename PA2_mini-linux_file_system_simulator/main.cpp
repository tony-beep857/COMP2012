#include <iostream>
#include <string>

#include "OperatingSystem.h"

int main() {
  OperatingSystem sys;
  std::string buf;

  while (true) {
    std::cout << "login: ";
    std::string username;
    if (!std::getline(std::cin, username) || username.empty())
      break;

    bool res = sys.login(username);
    if (!res) continue;

    ExecResult result{ExecResult::Continue};
    while (result == ExecResult::Continue) {
      sys.show_shell_prompt();
      if (!std::getline(std::cin, buf)) {
        return 0;
      }
      result = sys.execute(buf);
    }

    if (result == ExecResult::Shutdown)
      break;
  }
}
