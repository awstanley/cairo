# Cairo CMakeLists

This is a trivial Cairo builder.  It currently lacks most of the power of the configure.ac file, but it's all I need for what I do and I thought it worthwhile sharing.

## Usage:

    git clone https://github.com/awstanley/cairo
    cd cairo
    git submodule init
    git submodule update
    cd ..
    mkdir cairo-build
    cd cairo-build
    cmake ../cairo

After executing that, build using the project files generated.

## TODO

Add a lot of options for handling all of the Cairo feature definitions, replacing them with 0 and 1 values instead of the current fixes features.