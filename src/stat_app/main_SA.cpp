#include <iostream>

#include "stat_controller.hpp"

namespace SA_cli {
inline constexpr int MIN_PORT = 1;
inline constexpr int MAX_PORT = 65535;
inline constexpr int MIN_N = 1;
inline constexpr int MIN_T = 1;
}  // namespace SA_cli

int main(int argc, const char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <port> <N> <T>\n";
    return 1;
  }

  int port, N, T;
  try {
    port = std::stoi(argv[1]);
    N = std::stoi(argv[2]);
    T = std::stoi(argv[3]);
  } catch (const std::exception& e) {
    std::cerr << "Invalid number format.\n";
    return 1;
  }

  if (port < SA_cli::MIN_PORT || port > SA_cli::MAX_PORT || N < SA_cli::MIN_N ||
      T < SA_cli::MIN_T) {
    std::cerr << "Invalid parameters. Port must be " << SA_cli::MIN_PORT << "-"
              << SA_cli::MAX_PORT << ", N and T > 0.\n";
    return 1;
  }

  stat_controller_api::setParams(static_cast<stat_controller::PortType>(port),
                                 static_cast<unsigned>(N),
                                 static_cast<unsigned>(T));

  stat_controller_api::work();
  return 0;
}