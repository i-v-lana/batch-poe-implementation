#! python
import subprocess
import os

# Define the commands
commands = [
    "git clone https://github.com/microsoft/vcpkg",
    r"./vcpkg/bootstrap-vcpkg.sh",
    r"./vcpkg/vcpkg install pkgconf",
    r"./vcpkg/vcpkg install gmp",
    r"./vcpkg/vcpkg install cryptopp",
    r"./vcpkg/vcpkg integrate install"
]

include_dir = "./../../include"
os.makedirs(include_dir, exist_ok=True)
print(f"Downloading the required libraries and storing them into {include_dir}")
print("The installer estimation time is 3 minutes.")
print("Warning: Prerequisites are autoconf, developer tools, g++ >= 6 and git")
print("Instructions for installing prerequisites:")
print("Autoconf, MacOS with homebrew installed (if you don't have homebrew, install autoconf with another package manager) : $ brew install autoconf && brew install automake")
print("Autoconf, Linux: $ sudo apt-get update && sudo apt-get install autoconf")
print("Developer tools, MacOS: $ xcode-select --install")
print("Developer tools, Debian, Ubuntu, popOS, and other Debian-based distributions: $ sudo apt-get update && sudo apt-get install build-essential tar curl zip unzip")
print("Developer tools, CentOS: $ sudo yum install centos-release-scl && sudo yum install devtoolset-7 && scl enable devtoolset-7 bash")

for cmd in commands:
    try:
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   universal_newlines=True, cwd=include_dir)
        for line in process.stdout:
            print(line, end='')
        for line in process.stderr:
            print(line, end='')
        process.wait()
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {cmd}")
        print(e)
        break
    else:
        print(f"Command executed successfully: {cmd}")

print("All commands executed.")
