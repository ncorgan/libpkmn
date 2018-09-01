LibPKMN
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/libpkmn/blob/master/LICENSE.txt)
[![Travis-CI Build](https://travis-ci.org/ncorgan/libpkmn.svg)](https://travis-ci.org/ncorgan/libpkmn)
[![AppVeyor Build](https://ci.appveyor.com/api/projects/status/github/ncorgan/libpkmn)](https://ci.appveyor.com/project/ncorgan/libpkmn)

The goal of LibPKMN is to be the most expansive library for editing, analyzing, and otherwise customizing main
series Pokémon games and saves.

Features:
 * Game save editing
 * Damage calculation
 * Pokémon breeding
 * Qt subclasses for easy GUI development

Supported games:
 * Red/Blue/Yellow
 * Gold/Silver/Crystal
 * Ruby/Sapphire/Emerald
 * FireRed/LeafGreen
 * Colosseum/XD

Supported languages:
 * C++
 * C
 * C#
 * Lua
 * Python
 * Ruby

Build Dependencies
-------------------------------------

* C/C++
  * Core Library, C API
    * [CMake](https://www.cmake.org) (2.8.9+)
    * [Boost](https://www.boost.org) (1.55+)
    * [Python](https://www.python.org) (2.7+ or 3.3+)
  * Qt Widgets:
    * [Qt](https://www.qt.io) (4/5)
* SWIG wrappers
  * Common dependencies
    * [SWIG](http://swig.org/)
      * Python 2 support: 3.0.8+
      * Ruby support: 3.0.3+
      * Others: 3.0.0+
    * [CppHeaderParser](https://pypi.python.org/pypi/CppHeaderParser/)
    * [PLY](https://pypi.python.org/pypi/ply)
  * C# ([.NET](https://www.microsoft.com/net)/[Mono](http://www.mono-project.com/))
  * [Lua](https://www.lua.org) (5.1+)
  * [Python](https://www.python.org) (2.7+ or 3.3+)
  * [Ruby](https://www.ruby-lang.org) (2.1.0+)
* Documentation
  * C++, C, C#, Python: [Doxygen](https://www.stack.nl/~dimitri/doxygen/)
  * Ruby: [RDoc](https://github.com/ruby/rdoc)

Testing
-------------------------------------

Using [Travis](https://travis-ci.org/ncorgan/libpkmn) and
[AppVeyor](https://ci.appveyor.com/project/ncorgan/libpkmn), every push to this
repository is run through extensive unit tests with the following configurations:

 * Ubuntu 14.04 (x64)
   * C/C++
     * GCC: 4.8.4, 5.5.0, 6.4.0, 7.3.0
     * Clang: 4.0.1, 5.0.0, 5.0.2
     * Boost: 1.55.0, 1.60.0
     * Qt: 4.8.6, 5.2.1
   * SWIG: 2.0.11, 3.0.2
   * C#: Mono 3.2.8
   * Lua: 5.1.5
   * Python: 2.7.6, 3.4.3
   * Ruby: 2.4.1
 * Windows Server 2012 R2 (x64)
   * C/C++: MSVC 2015 (Win32, x64)
     * Boost: 1.63.0, 1.65.0, 1.67.0
     * Qt: 5.10.1
   * SWIG: 3.0.9
   * C#: .NET 3.5
   * Lua: 5.1.5, 5.2.4, 5.3.2
   * Python: 3.5.3

Tests are run with the following frameworks:
 * C++: [Google Test](https://github.com/google/googletest), [QtTest](http://doc.qt.io/qt-5/qttest-index.html)
 * C: [Unity](https://github.com/ThrowTheSwitch/Unity)
 * C#: [NUnit](https://www.nunit.org/)
 * Lua: [Luaunit](https://github.com/bluebird75/luaunit)
 * Python: [unittest](https://docs.python.org/3/library/unittest.html) + [nose-parameterized](https://pypi.python.org/pypi/nose-parameterized/)
 * Ruby: [test-unit](https://github.com/test-unit/test-unit) + [minitest](https://github.com/seattlerb/minitest)

All C/++ source files are analyzed with [CppCheck](http://cppcheck.sourceforge.net/) and [Clang-Tidy](http://clang.llvm.org/extra/clang-tidy/).

Acknowledgments
-------------------------------------

* Lexy "Eevee" Munroe ([@eevee](https://github.com/eevee)), whose extensive SQLite database on all things Pokémon underlies the majority of this code's functionality.
* Michael Bond ([@codemonkey85](https://github.com/codemonkey85)), for consulting on game mechanics.
* TuxSH ([@TuxSH](https://github.com/TuxSH)), whose library [LibPkmGC](https://github.com/TuxSH/PkmGCTools) underlies LibPKMN's Gamecube save editing functionality.

This software is in no way affiliated with or endorsed by Nintendo, Creatures Inc. or Game Freak Inc, and is created for solely recreational and non-profit use. Pokémon © 2002- Pokémon. © 1995- Nintendo/Creatures Inc./GAME FREAK inc. TM, ® and Pokémon character names are trademarks of Nintendo.

README last updated: 2018/08/31
