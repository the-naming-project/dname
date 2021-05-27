# dname

The core deterministic naming library.

Composed of two components

 - `libdname.so` The library shared object
 - `dname` The cli executable

# Building libdname.so

To build and install `libdname.so` run the following:

```bash
git clone git@github.com:kris-nova/dname
./configure
cd build
cmake ..
make
sudo make install
```

# Dname

To run an example program using the library

```bash
cd dname
make
export LD_LIBRARY_PATH=/usr/local/lib
./dname <any input>
make clean
```

# Running

```bash
[nova@emma dname]$ dname boops
[Input ]: boops
[Output]: 06afe0c85ac2d2f9463c626ad465e796de3adf91586e2847e484646400000000
```