# Installation and User Documentation - batch PoEs implementation 

This repository contains the source code for the implementation of [the Random Exponents and the Random Subsets Protocols by Rotem](https://eprint.iacr.org/2021/1209), [the Hybrid and the Bucket Protocols](https://eprint.iacr.org/2024/145) in C++.
It builds up on the implementation of [Wesolowski's PoE by Attias, Vigneri and Dimitrov of the Network Team of IOTA Foundation](https://github.com/iotaledger/vdf/blob/master).

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

After running the appropriate script, you can run your code with CMake from one of the IDEs (CLion/Visual Studio), see the details below in User Documentation:


# User Documentation

This project implements various batching techniques for the [Wesolowski protocol](https://eprint.iacr.org/2018/623).

You can build the program using CMake and then run it with the following command:
```bash
./src protocol_type logt experiments instances_per_exp
```
where 
- `protocol_type`: The batching protocol you want to use (naive/bucket/hybrid/exponential/subset/all_but_naive/all)
- `logt`: Logarithm of the T in `x^(2^T) = y`.
- `experiments`: Number of experiments to run.
- `instances_per_exp`: Number of instances to batch per experiment.

All security parameters for the protocols are hardcoded, the modulus for the calculations is generated in the code as well.

The instances for batching are also generated in the code.

## Protocol_type=naive as Naive Approach
For every instance runs the Wesolowski protocol separately.

## Protocol_type=exponential as the Random Exponents Protocol
Represents the [Random Exponents batching approach](https://eprint.iacr.org/2021/1209) from Lior Rotem's paper (the random exponent compiler). 

See the [programmer_documentation.md](https://github.com/i-v-lana/batch-poe-implementation/blob/master/Programmer_documentation.md) for further details.

## Protocol_type=subset as the Random Subsets Protocol
Represents the [Random Subsets batching protocol](https://eprint.iacr.org/2021/1209) from Lior Rotem's paper (the random subset compiler). 

See the [programmer_documentation.md](https://github.com/i-v-lana/batch-poe-implementation/blob/master/Programmer_documentation.md) for further details.

## Protocol_type=bucket as the Bucket Protocol
See the [programmer_documentation.md](https://github.com/i-v-lana/batch-poe-implementation/blob/master/Programmer_documentation.md) for further details.

## Protocol_type=hybrid as the Hybrid Protocol
See the [programmer_documentation.md](https://github.com/i-v-lana/batch-poe-implementation/blob/master/Programmer_documentation.md) for further details.

## Protocol_type=all
Runs all the above types for the same parameters and instances. Used for the comparison of the protocols.

## Protocol_type=all_but_naive
Runs all the above types of protocols except for the `naive` for the same parameters and instances. Used for the comparison of the protocols.

In the folder "theoretical-analysis", there is the script (verification_costs.py) for calculating and plotting the theoretical evaluation of the expected number of group multiplications for the Random Exponents, Bucket and Hybrid Protocols.
In the folder "experiments", there are the scripts "run_experiments.py" and "results_processing.py", which run the C++ implementation of the protocols with provided parameters and plot the results. However, the C++ can also be run manually, as described above.


