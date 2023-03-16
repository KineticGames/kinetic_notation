job("CMake Build & Test") {
  startOn {
    gitPush { enabled = true }
    codeReviewOpened {
        branchToCheckout = CodeReviewBranch.MERGE_REQUEST_SOURCE
      }
    }
    container(displayName = "CMake", image = "ubunu")
    {
      shellScript {
        content = """
        apt install cmake
        cmake -B build --Release
        cmake --build build --Release
        ctest --test-dir build/test
        """
        }
      }
  }
