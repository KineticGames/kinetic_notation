job("CMake Build & Test") {
    startOn {
        gitPush { enabled = true }
    }
    parameters {
        text("build-type", value = "Release")
    }
    container(displayName = "CMake", image = "ubuntu") {
        cache {
            localPath = "build/*"
        }
        shellScript {
            content = """
                apt update
                apt install build-essential cmake -y
                cmake -B build -DCMAKE_BUILD_TYPE={{ build-type }}
                cmake --build build --config {{ build-type }}
                ctest --test-dir build/test -C {{ build-type }}
        """
        }
    }
}
