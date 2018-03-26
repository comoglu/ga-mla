# Geoscience Australia SeisComP3 Modules/Plugins

Various plugins developed for [SeisComP3](https://www.seiscomp3.org/) by 
Geoscience Australia.


## Building

### Assumptions

- you are going to work in your home directory (hence all the `cd` commands). If
  you are going to work somewhere else, please *cd* to that directory instead.

- you are using Ubuntu. The main change if you are not using Ubuntu, would be
  to change the apt-get calls. The Redhat/CentOS package equivalents for many
  of the packages are stated [here](https://github.com/SeisComP3/seiscomp3#dependencies).


### Process

- Get the prerequisites (this includes the dependencies mentioned in
  [the SeisComP3 README](https://github.com/SeisComP3/seiscomp3/blob/master/README.md))

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
        libpython-dev \
        python-m2crypto \
        libqt4-dev 
    ```

  If you want to use ccmake (as per the 
  [compilation instructions](https://github.com/SeisComP3/seiscomp3/blob/master/README.md#compiling),
  then you also want

    ```
    sudo apt-get install -y cmake-curses-gui
    ```

  I don't do this and use cmake directly as per my example below.


- Get the SeisComP3 source code

    ```
    cd
    git clone https://github.com/SeisComP3/seiscomp3.git
    ```

  Note that you will need a version which contains commit 
  [d2b631715a4b](https://github.com/SeisComP3/seiscomp3/commit/d2b631715a4b68e8291801bef8cb1b1e808a30ee),
  which introduces a change which the MLa magnitude plugin depends on.
  

- Go to the src directory and clone this repository into the directoy 'ga'

    ```
    cd
    cd seiscomp3/src
    git clone https://github.com/GeoscienceAustralia/seiscomp3.git ga
    # or if you are are going to commit back use:
    # git clone git@github.com:GeoscienceAustralia/seiscomp3.git ga
    ```


- Return to the top level directory and follow the build instructions in the
  [README](https://github.com/SeisComP3/seiscomp3/blob/master/README.md) of the
  the [SeisComP3](https://github.com/SeisComP3/seiscomp3.git) repository. In
  brief, I do this with

    ```
    cd
    cd seiscomp3
    mkdir build
    cd build
    cmake -G "Unix Makefiles" ..
    make
    ```

  If you only want to build the GA plugins and their dependencies

    ```
    cd 
    cd seiscomp3/src/ga
    make
    ```

  This still builds quite a bit of stuff, but does save some time.

  I'm not sure if there is an easy way to grap only the files required for the
  GA plugins, everything seems to end up in the directories under the top level
  build directory.


- For development, you probably want to run `make install` from the top level
  directory to give you a working SeisComP3 system.
