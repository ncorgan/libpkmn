LibPKMN
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/libpkmn/blob/master/LICENSE.txt)
[![Travis-CI Build](https://travis-ci.org/ncorgan/libpkmn.svg)](https://travis-ci.org/ncorgan/libpkmn)
[![AppVeyor Build](https://ci.appveyor.com/api/projects/status/github/ncorgan/libpkmn)](https://ci.appveyor.com/project/ncorgan/libpkmn)

The goal of LibPKMN is to be the most portable, intuitive, and expansive save editing and analysis
library for the main series Pokémon games.

Feature Roadmap:
 * C# API
 * Java API
 * Lua API

Build Dependencies
-------------------------------------
 * Core C++ Library (required), C API (optional)
   * [CMake](https://www.cmake.org) (minimum version 2.8.9)
   * [Boost](https://www.boost.org) (minimum version 1.55)
   * [Python](https://www.python.org) (both 2 and 3 supported)
 * Qt Widgets (optional)
   * [Qt](https://www.qt.io) (both 4 and 5 supported)
 * Lua API (optional)
   * [SWIG](http://swig.org/) (minimum version 2.0.11)
   * [CppHeaderParser](https://pypi.python.org/pypi/CppHeaderParser/)
   * [PLY](https://pypi.python.org/pypi/ply)
 * Python API (optional)
   * [SWIG](http://swig.org/)
   * [CppHeaderParser](https://pypi.python.org/pypi/CppHeaderParser/)
   * [PLY](https://pypi.python.org/pypi/ply)
 * API Documentation (optional)
   * [Doxygen](https://www.stack.nl/~dimitri/doxygen/)

Testing
-------------------------------------

Using [Travis](https://travis-ci.org/ncorgan/libpkmn) and
[AppVeyor](https://ci.appveyor.com/project/ncorgan/libpkmn), every push to this
repository is automatically tested under the following configurations:

 * GCC 4.8.4, Python 2.7.6
 * Clang 3.5, Python 2.7.6
 * MSVC 2015 (x86, x64), Python 2.7.12
 * MSVC 2015 (x86, x64), Python 3.5.2

Tests are run with the following frameworks:
 * C++: [Boost.Test](http://www.boost.org/doc/libs/1_62_0/libs/test/doc/html/index.html)
 * C: [Unity](https://github.com/ThrowTheSwitch/Unity)
 * Lua: [Luaunit](https://github.com/bluebird75/luaunit)
 * Python: [unittest](https://docs.python.org/3/library/unittest.html)

All source files are analyzed with [CppCheck](http://cppcheck.sourceforge.net/).

README last updated: 2016/12/29
