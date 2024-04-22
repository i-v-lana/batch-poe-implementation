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
print("The installer estimation time is 40 minutes.")


# Execute the commands
for cmd in commands:
    try:
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   universal_newlines=True, cwd=include_dir)
        for line in process.stdout:
            print(line, end='')  # Print stdout
        for line in process.stderr:
            print(line, end='')  # Print stderr
        process.wait()  # Wait for the process to finish
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {cmd}")
        print(e)
        break
    else:
        print(f"Command executed successfully: {cmd}")

print("All commands executed.")
