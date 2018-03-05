# Project starter

Cpp project

This device server is able to control <b>Tango</b> components (database, device servers, clients...).
It is able to start or stop and to report the status of these components.
## Cloning 

```
git clone  git@github.com:tango-controls/starter
```

## Documentation 

See [Documentation](http://www.tango-controls.org/developers/dsc/ds/423/) for more details.

See also [Astor](http://tango-controls.readthedocs.io/en/latest/tools-and-extensions/astor/index.html?highlight=Astor)

## Building and Installation

See the [INSTALL.md](INSTALL.md) file for detailed instructions on how to build and install libhdbpp-cassandra. **(Delete If Does Not Apply)**

### Dependencies

The project has the following dependencies.

#### Project Dependencies

* Tango Controls 9 or higher.
* omniORB release 4 or higher.
* libzmq - libzmq3-dev or libzmq5-dev.

#### Toolchain Dependencies

* C++11 compliant compiler.
* CMake 3.0 or greater is required to perform the build. **(Delete If Does Not Apply)**

### Build

Instructions on building the project.

CMake skeleton example:

```
cd project_name
mkdir build
cd build
cmake ../
make
```

Make skeleton example:

```
cd project_name
make
```

