# Installation and Usage Instructions

## Step 0: Clonning this repository
Create a folder with some name *name*. Clone the repository as a subfolder of *name*. Installer will then create a folder *include* in *name*.

## Step 1: Choose the Correct Installer

In the "installer" folder, you'll find two Python scripts:

- **main_unix.py**: Use this for Unix-based systems like MacOS and Linux.
- **main_windows.py**: Use this for Windows systems.

Run one of them depending on your operating system.

### For Unix-based Systems (main_unix.py):

```bash
$ python3 main_unix.py
```

- This script downloads the required libraries and stores them into the folder *include* as a subfolder of *name* from step 0.
- Estimated installation time: 3 minutes.

#### Prerequisites:
- **Autoconf**: 
    - MacOS with Homebrew: 
        ```bash
        $ brew install autoconf && brew install automake
        ```
    - Linux: 
        ```bash
        $ sudo apt-get update && sudo apt-get install autoconf
        ```
- **Developer tools**:
    - MacOS:
        ```bash
        $ xcode-select --install
        ```
    - Debian, Ubuntu, Pop!_OS, and other Debian-based distributions:
        ```bash
        $ sudo apt-get update && sudo apt-get install build-essential tar curl zip unzip
        ```
    - CentOS:
        ```bash
        $ sudo yum install centos-release-scl && sudo yum install devtoolset-7 && scl enable devtoolset-7 bash
        ```

### For Windows Systems (main_windows.py):

```bash
$ python3 main_windows.py
```

- This script downloads the required libraries and stores them into the folder *include* as a subfolder of *name* from step 0.

#### Prerequisites:
- Windows 7 or newer
- Git
- Visual Studio 2015 Update 3 or greater

> Note: You can compile the code without using Visual Studio, but then you should run CMake with Visual Studio compiler.

## Step 2: Running Your Code

After running the appropriate script, you can run your code with CMake from one of the IDEs (CLion/Visual Studio).
