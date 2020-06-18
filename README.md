# Geoscience Australia SeisComP Modules/Plugins

Various plugins developed for [SeisComP](https://www.seiscomp.de/) by
Geoscience Australia.


## Building

### Assumptions

- You are using Ubuntu. The main change if you are not using Ubuntu, would be
  to change the apt-get calls to the appropriate package manager (and
  dependency package names) for your operating system.

- You want to build for SeisComP >= 4.0.0 and Python 3.


### Process

- Get the prerequisites (this includes the dependencies mentioned in
  [the SeisComP README](https://github.com/SeisComP/seiscomp/blob/master/README.md))

    ```
    sudo apt-get install -y \
        cmake \
        build-essential \
        gfortran \
        flex \
        libxml2-dev \
        libboost-all-dev \
        libssl-dev \
        libncurses-dev \
        libmysqlclient-dev \
        python3-dev \
        python3-numpy \
        qtbase5-dev
    ```

  If you want to use ccmake (as per the
  [compilation instructions](https://github.com/SeisComP/seiscomp/blob/master/README.md#compiling)),
  then you also want

    ```
    sudo apt-get install -y cmake-curses-gui
    ```

  I don't do this and use cmake directly as per my example below.

- Ensure python3 is set as your default system python, e.g. on Ubuntu 18.04:
    ```
    sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.6m 3
    sudo update-alternatives --config python
    ```

- Get the SeisComP source code by following the instructions in the seiscomp readme.

  If you only want to build the GA plugins, you should just need the `seiscomp` and
  `common` repositories.

- Go to the src directory and clone this repository into the directory 'ga'.


- Return to the top level directory and follow the build instructions in the
  [README](https://github.com/SeisComP/seiscomp/blob/master/README.md) of the
  the [SeisComP](https://github.com/SeisComP/seiscomp.git) repository.

  If you only want to build the GA plugins and their dependencies, just use
  cmake to generate the makefiles, and then run:

    ```
    cd src/ga
    make
    ```

  This still builds quite a bit of stuff, but does save some time.

  I'm not sure if there is an easy way to grap only the files required for the
  GA plugins, everything seems to end up in the directories under the top level
  build directory.


- For development, you probably want to run `make install` from the build
  directory to give you a working SeisComP system.
