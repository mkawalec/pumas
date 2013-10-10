Pumas and Hares
=====

Programming Skills project @ Edinburgh University, 2013

Programming Style
====

We are generally following the [kernel style guide](https://www.kernel.org/doc/Documentation/CodingStyle), with the exception of indenting by 4, *not* 8, spaces (I think 8 spaces is totally unreadable with classes (do you agree?)

Documentation
====

We'll be using Doxygen, the output of which will probably be parsed by Sphinx, if I have the time. Please write the documentation using [markdown](http://daringfireball.net/projects/markdown/syntax), which will enable easy HTML code generation. You can also find it helpful to [look at the Doxygen specific markdown docs](http://www.stack.nl/~dimitri/doxygen/manual/markdown.html).

Building the code
====

We are using [CMake](http://www.cmake.org/) to govern the build process, though I don't think that we should support any platforms different from Linux/gcc. If you wish to add support for any other platform, feel free to do it in CMakeLists.txt

In order to build the code, you need to create a build directory, generate the makefile and build:

    mkdir build
    cd build
    cmake ..
    make -j3

which should produce a 'solver' executable for you.

Tests
====

We will most probably be using the unit test framework from [Boost](http://www.boost.org/). It is added as a compile-time requirement.
