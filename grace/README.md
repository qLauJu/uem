# Grace Lang

## Installing Bison
Install Bison 3.1 from this [link](http://ftp.gnu.org/gnu/bison/bison-3.1.tar.xz).

Use the following commands to install:
```bash
tar -xvf bison-3.1.tar.xz
cd bison-3.1/
./configure && make && make install
```

## Installing Flex 
Install Flex using `sudo apt-get install flex` (We are using Flex v.2.6.0).

## Running the project
Clone the repository at `git clone https://github.com/grsouza/grace.git`.
```bash
cd grace
make
./grace Name-of-file.cz
```
