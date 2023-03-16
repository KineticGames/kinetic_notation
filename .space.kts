job("CMake Build & Test") {
  startOn {
    gitPush { enabled = true }
    }
  parameters {
    text("build-type", value = "Release")
    }
    container(displayName = "CMake", image = "ubuntu")
    {
      shellScript {
        content = """
        apt update
        apt install cmake -y
        cmake -B build -DCMAKE_BUILD_TYPE={{ build-type }}
        cmake --build build --config {{ build-type }}
        ctest --test-dir build/test -C {{ build-type }}
        """
        }
      }
  }
