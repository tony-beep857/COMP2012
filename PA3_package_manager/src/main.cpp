#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "package_manager.h"

using std::operator""s;

namespace {
/**
 * @brief Prints the command-line usage to standard error.
 *
 * @param program The program name, as invoked.
 */
void print_usage(const char* program) {
  std::cerr << "Usage: " << program << " [--local-index <file>] [--remote-index <file>] [command]\n"
            << "  --local-index <file>    local index (default: $PWD/index; absent means nothing\n"
            << "                          is installed, written on the first change)\n"
            << "                          '<pkgname> <pkgver> [explicit]' per line\n"
            << "  --remote-index <file>   remote index (default: packages/remote/default/index),\n"
            << "                          one package name per line\n"
            << "Commands (default: list):\n"
            << "  list                    print the loaded state\n"
            << "  install <pkg>           install a package explicitly\n"
            << "  uninstall <pkg>         remove a package and its unused dependencies\n"
            << "  upgrade                 upgrade installed packages to newer remote versions\n"
            << "  prune                   remove dependencies nothing requires\n"
            << "  orphans                 list installed packages no remote provides\n";
}

/**
 * @brief Names a status as it is printed.
 *
 * @param status The operation outcome.
 * @return The printed name.
 */
std::string_view status_name(Status status) {
  switch (status) {
    case Status::SUCCESS:
      return "SUCCESS";
    case Status::CONFLICT:
      return "CONFLICT";
    case Status::BREAKS_DEPS:
      return "BREAKS_DEPS";
    case Status::NOT_FOUND:
      return "NOT_FOUND";
    case Status::ALREADY_INSTALLED:
      return "ALREADY_INSTALLED";
  }
  return "UNKNOWN";
}

/**
 * @brief Prints the loaded remote and local state, one installed package per line.
 *
 * @param manager The manager whose state to print.
 */
void print_state(const PackageManager& manager) {
  std::cout << manager.get_remote().get_path().string() << ": " << manager.get_remote().list().size()
            << " package(s)\n";

  const LocalRepository& local{manager.get_local()};
  std::cout << local.get_path().string() << ": " << local.list().size() << " installed\n";

  std::vector<const Package*> packages{local.list()};
  std::sort(packages.begin(), packages.end(), [](const Package* left, const Package* right) {
    return left->get_name() < right->get_name();
  });

  for (const Package* package : packages) {
    std::cout << "  " << package->get_name() << " " << static_cast<std::string>(package->get_version())
              << (local.is_explicit(package->get_name()) == true ? " explicit" : "") << "\n";
  }
}
}  // namespace

int main(int argc, const char* argv[]) {
  if (argc >= 2 && (std::string_view{argv[1]} == "--help" || std::string_view{argv[1]} == "-h")) {
    print_usage(argv[0]);
    return EXIT_SUCCESS;
  }

  std::filesystem::path local_index;
  std::filesystem::path remote_index;
  std::string command{"list"s};
  std::vector<std::string> command_arguments;

  int argument_index{1};
  for (; argument_index < argc; ++argument_index) {
    const std::string_view argument{argv[argument_index]};
    if (argument == "--local-index" || argument == "--remote-index") {
      if (argument_index + 1 >= argc) {
        std::cerr << "Error: " << argument << " needs a path\n";
        return EXIT_FAILURE;
      }

      if (argument == "--local-index") {
        local_index = argv[++argument_index];
      } else {
        remote_index = argv[++argument_index];
      }
      continue;
    }

    if (argument.empty() || argument.front() != '-') {
      break;
    }

    std::cerr << "Error: Unknown argument " << argument << "\n";
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  if (argument_index < argc) {
    command = argv[argument_index++];
    for (; argument_index < argc; ++argument_index) {
      command_arguments.emplace_back(argv[argument_index]);
    }
  }

  if (local_index.empty()) {
    local_index = std::filesystem::current_path() / "index";
  }
  if (remote_index.empty()) {
    remote_index = std::filesystem::path{"packages"} / "remote" / "default" / "index";
  }

  Expected<PackageManager, std::string> manager{PackageManager::create(local_index, remote_index)};
  if (!manager) {
    std::cerr << "Error: " << manager.error() << "\n";
    return EXIT_FAILURE;
  }

  int exit_code{EXIT_SUCCESS};
  if (command == "list") {
    if (!command_arguments.empty()) {
      std::cerr << "Error: list takes no arguments\n";
      return EXIT_FAILURE;
    }
  } else if (command == "install") {
    if (command_arguments.size() != 1) {
      std::cerr << "Error: install needs exactly one package name\n";
      return EXIT_FAILURE;
    }
    const Status status{manager->install(command_arguments.front(), true)};
    std::cout << "install " << command_arguments.front() << ": " << status_name(status) << "\n";
    exit_code = status == Status::SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
  } else if (command == "uninstall") {
    if (command_arguments.size() != 1) {
      std::cerr << "Error: uninstall needs exactly one package name\n";
      return EXIT_FAILURE;
    }
    const Status status{manager->uninstall(command_arguments.front())};
    std::cout << "uninstall " << command_arguments.front() << ": " << status_name(status) << "\n";
    exit_code = status == Status::SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
  } else if (command == "upgrade") {
    if (!command_arguments.empty()) {
      std::cerr << "Error: upgrade takes no arguments\n";
      return EXIT_FAILURE;
    }
    const Status status{manager->upgrade()};
    std::cout << "upgrade: " << status_name(status) << "\n";
    exit_code = status == Status::SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
  } else if (command == "prune") {
    if (!command_arguments.empty()) {
      std::cerr << "Error: prune takes no arguments\n";
      return EXIT_FAILURE;
    }
    std::cout << "prune: removed " << manager->prune() << " package(s)\n";
  } else if (command == "orphans") {
    if (!command_arguments.empty()) {
      std::cerr << "Error: orphans takes no arguments\n";
      return EXIT_FAILURE;
    }
    const std::vector<std::string> orphans{manager->orphans()};
    if (orphans.empty()) {
      std::cout << "orphans: none\n";
    } else {
      std::cout << "orphans: " << orphans.size() << "\n";
      for (const std::string& orphan : orphans) {
        std::cout << "  " << orphan << "\n";
      }
    }
  } else {
    std::cerr << "Error: Unknown command " << command << "\n";
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  const bool mutating{command == "install" || command == "uninstall" || command == "upgrade" || command == "prune"};
  if (mutating && exit_code == EXIT_SUCCESS) {
    const std::optional<std::string> saved{manager->get_local().save(local_index)};
    if (saved) {
      std::cerr << "Error: " << *saved << "\n";
      return EXIT_FAILURE;
    }
  }

  print_state(*manager);
  return exit_code;
}
