#! python
import subprocess
import os


# Define the commands
commands = [
    "git clone https://github.com/microsoft/vcpkg",
    r"./vcpkg/bootstrap-vcpkg.bat",
    r"./vcpkg/vcpkg install pkgconf",
    r"./vcpkg/vcpkg install gmp",
    r"./vcpkg/vcpkg install cryptopp",
    r"./vcpkg/vcpkg integrate install"
]

include_dir = "./../../include"
os.makedirs(include_dir, exist_ok=True)
print(f"Downloading the required libraries and storing them into {include_dir}.")
print("The installer estimation time is 40 minutes.")
print("Warning: Prerequisites are Windows 7 or newer, git, Visual Studio 2015 Update 3 or greater.")
print("Warning: You can compile the code without using Visual Studio, but then you should run CMake with Visual Studio compiler.")


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
