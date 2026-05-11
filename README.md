# Popcorn OS

Popcorn OS is a simple, single-threaded OS with a monolithic kernel. Unlike many hobby OSes, it reads apps from disk! It currently has:

- A command processor
- Simple graphical demos
- A very hopeful maintainer
    
It is built as a UEFI application and uses a service table instead of traditional syscalls. 

## Compilation

Popcorn OS currently uses a hybrid toolchain:

1. The kernel must be compiled with MSVC (Microsoft Visual C++).
   The kernel depends on MSVC's handling of certain calling conventions and startup code (as in, it doesn't need them); GCC/Clang builds are not supported at this time.

2. Applications must be compiled with GCC if you want proper behaviour. (MSVC works, but you can't make non-inline functions. GCC works! ...except you can't make inline functions, the opposite problem. GCC needs non-inline functions to be declared before pop_main, obviously, but only defined after pop_main.)

This means the build process is not fully cross-platform.

On Windows, you need both MSVC and GCC installed.

On Linux, you can build the applications with GCC, but the kernel cannot be built without MSVC.

The kernel relies on MSVC‑specific behavior to produce a working EFI binary. Attempts to build the kernel with GNU toolchains on Windows fail due to assembler and ABI incompatibilities. Using MSVC ensures reliable builds and correct execution under OVMF/QEMU.

I'd like to unify the toolchain in the future (e.g. by massaging gnu-efi), but for now MSVC is mandatory for the kernel. If you can find a way to fix it, please do! Submit a pull request, *please*.

To compile it, simply clone the repo with all prerequisites and run:
``` bash
make
```
Or run:
``` bash
make help
```
to get more information.

When you first run `make`, it will download all the dependencies. This was done so this repo only contains code. 
And then it will run normally. It will delete all previous build artifacts and compile the kernel, then the apps, and then run it directly from a folder using QEMU. Unless you do `make prod`, in which it creates a `.img` file first and then boots from that.

## Application format

Apps are packaged as flat binaries (`.bin`) that have a function at address 0 which takes `(pop_Services* svc, int argc, CHAR16** argv)`.
This function is commonly called `pop_main`. The service table is very documented in `popcorn.h`.