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
 * Objective-C API

Testing
-------------------------------------

Using [Travis](https://travis-ci.org/ncorgan/pksav) and
[AppVeyor](https://ci.appveyor.com/project/ncorgan/pksav), every commit in this
repository is automatically tested under the following configurations:

 * Compile, unit test:
   * GCC 4.8.4, Python 2.7.6
   * Clang 3.5, Python 2.7.6
   * AppleClang 7.3.0, Python 2.7.12
   * MSVC 2015 (x86, x64), Python 2.7.12
   * MSVC 2015 (x86, x64), Python 3.5.2

All source files are analyzed with [CppCheck](http://cppcheck.sourceforge.net/).

README last updated: 2016/11/25
