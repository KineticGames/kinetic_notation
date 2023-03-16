job("CMake Build & Test") {
  startOn {
    gitPush { enabled = true }
    }
    container(displayName = "CMake", image = "ubuntu")
    {
      shellScript {
        content = """
        apt update
        apt install cmake -y
        cmake -B build --Release
        cmake --build build --Release
        ctest --test-dir build/test
        """
        }
      }
  }
