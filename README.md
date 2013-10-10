Pumas and Hares
=====

Programming Skills project @ Edinburgh University, 2013

## Programming Style


We are generally following the [kernel style guide](https://www.kernel.org/doc/Documentation/CodingStyle), with the exception of indenting by 4, *not* 8, spaces (I think 8 spaces is totally unreadable with classes (do you agree?).

## Documentation

We'll be using Doxygen, the output of which will probably be parsed by Sphinx, if I have the time. Please write the documentation using [markdown](http://daringfireball.net/projects/markdown/syntax), which will enable easy HTML code generation. You can also find it helpful to [look at the Doxygen specific markdown docs](http://www.stack.nl/~dimitri/doxygen/manual/markdown.html).

## Building

We are using [CMake](http://www.cmake.org/) to govern the build process, though I don't think that we should support any platforms different from Linux/gcc. If you wish to add support for any other platform, feel free to do it in CMakeLists.txt

### Building the application

In order to build the code, you need to create a build directory, generate the makefile and build:

    mkdir build
    cd build
    cmake ..
    make -j3

which should produce 'solver' and 'test-suite' executables for you.

## Tests

We will most probably be using the unit test framework from [Boost](http://www.boost.org/). It is added as a compile-time requirement, so you need to have it installed.

A good how-to on using Boost tests can be found [in the Boost docs](http://www.boost.org/doc/libs/1_35_0/libs/test/doc/components/utf/index.html). The first three examples are mostly worth the read.

Please do *not* mark a task as done in Trello if it doesn't have at least a few tests to prove its correctness!

### Building the documentation

To do that you need to be inside the build directory and make doc:

    cd build
    make doc

