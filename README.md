# libname

The core C library, tests, and examples.

# Building from source

To build and install `libname.so` run the following:

```bash
git clone git@github.com:kris-nova/libname
./configure
cd build
make
sudo make install
```

# Example usage

To run an example program using the library

```bash
cd examples
make
export LD_LIBRARY_PATH=/usr/local/lib
./namestring
make clean
```