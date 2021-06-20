# dname 0.1.2

Command line tool and shared object that can be used to deterministically calculate an 8 byte hash given an arbitrary input.
The main library also ships with runtime lookup features that will generate a name based on the state of a linux system at runtime.

Hardware, The state of the kernel, the state of the container, and kubernetes metrics are used to calculate the input for the hash.

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
[nova@emma dname]$ dname foxy
catnip-chiropractor-hayed-carpentered
[nova@emma dname]$ dname
estrus-ferrum-alexandr-slosh
[nova@emma dname]$ dname -j
{
        "input": "2019B9803731-0",
        "name": "estrus-ferrum-alexandr-slosh",
        "hex": "5daec0552d19dcf656291a68b4b64abcf953da09c3cfcca37771ffffffff"
}
[nova@emma dname]$
```