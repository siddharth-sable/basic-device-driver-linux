# Basic Device Driver for Echoing User Input

This is a simple Linux kernel module that serves as a basic device driver to echo back user input through a `/proc` entry. When a user writes a string to the `/proc/pyjama_driver` file, the driver will store it and return the same string when read.

Please use multipass or any virtual machine for trying it out - in order to avoiding playing with our own kernel.

## Table of Contents

- [Basic Device Driver for Echoing User Input](#basic-device-driver-for-echoing-user-input)
  - [Table of Contents](#table-of-contents)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Debugging](#debugging)
  - [Cleanup](#cleanup)
    - [Verify Removal](#verify-removal)
  - [License](#license)

## Prerequisites

Before building and running this driver, ensure you have the following installed:

- **Linux Kernel Headers**: The headers for the kernel you are running. You can install them using the package manager:
  - For Debian/Ubuntu: 
    ```bash
    sudo apt-get install linux-headers-$(uname -r)
    ```
  - For Red Hat/Fedora:
    ```bash
    sudo dnf install kernel-devel-$(uname -r)
    ```

- **Build Essentials**: You need to have the build tools installed. On Debian/Ubuntu, you can install it using:
  ```bash
  sudo apt-get install build-essential
  ```

## Installation

1. **Clone the Repository** (if using Git):
    ```bash
    git clone https://github.com/siddharth-sable/basic-device-driver-linux
    cd basic-device-driver-linux
    ```

2. **Compile the Driver**:
    Create a file named `Makefile` in the same directory as the source file (e.g., `pyjama_driver.c`) with the following content:

    ```makefile
    obj-m += pyjama_driver.o

    all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

    clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
    ```

    Then, run:
    ```bash
    make
    ```

3. **Load the Module**:
    Use the following command to load the module into the kernel:
    ```bash
    sudo insmod ldd.ko
    ```

4. **Verify the Module is Loaded**:
    Check if the module is loaded successfully using:
    ```bash
    lsmod | grep ldd
    ```

## Usage

1. **Writing to the Device**:
    Use `echo` to write a string to the `/proc` entry:
    ```bash
    echo "Hi" > /proc/pyjama_driver
    ```

2. **Reading from the Device**:
    Use `cat` to read back the string:
    ```bash
    cat /proc/pyjama_driver
    ```

3. **Expected Output**:
    If you write "Hi" and then read from the `/proc` file, you should see:
    ```
    Hi
    ```

## Debugging

- **Kernel Logs**:
    Use `dmesg` to view the kernel logs, which will show messages from your driver:
    ```bash
    dmesg | tail -n 20
    ```

- **Print Statements**:
    The driver uses `printk` to log various actions. You can change the log level by prefixing `printk` with different levels (e.g., `KERN_INFO`, `KERN_ERR`) to control what gets logged.

- **Error Checking**:
    Ensure to check the return values of functions like `copy_to_user()` and `copy_from_user()`. Log any errors to help troubleshoot issues.

## Cleanup

To remove the driver from the kernel, run:
```bash
sudo rmmod ldd
```

### Verify Removal
Check if the module has been removed using:
```bash
lsmod | grep ldd
```

## License

This driver is released under the GPL (General Public License). See the `LICENSE` file for more details.
