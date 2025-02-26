# PracticeOS

This is a *practice* version of the MellOS kernel. Its disk drivers and file system code are substituted by stubs. This way you can write your own without caring too much about the infrastructure.

## Building

You will find scripts to build the GCC cross compiler in A_Setup. This can take a lot of time (like 30 min).

- If you're using a Debian derivative, run the `setup-gcc-debian.sh` script. 
- If you're using Arch, run the `setup-gcc-arch.sh` script. 
- If you're using something else, you may either use an emulator or make your own script (and make a pull request)

If you're on Windows, you may use a Linux virtual machine as your development environment.