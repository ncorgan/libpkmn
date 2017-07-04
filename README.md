LibPKMN
===============================================

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/libpkmn/blob/master/LICENSE.txt)
[![Travis-CI Build](https://travis-ci.org/ncorgan/libpkmn.svg)](https://travis-ci.org/ncorgan/libpkmn)
[![AppVeyor Build](https://ci.appveyor.com/api/projects/status/github/ncorgan/libpkmn)](https://ci.appveyor.com/project/ncorgan/libpkmn)

The goal of LibPKMN is to be the most expansive library for editing main series Pokémon game saves, as well
as analyzing all aspects of the games, allowing for full customization of the game's Pokémon.

Supported games:
 * Red/Blue/Yellow
 * Gold/Silver/Crystal
 * Ruby/Sapphire/Emerald
 * FireRed/LeafGreen
 * Colosseum/XD

Features in development:
 * Damage calculations
 * Generation IV save support
 * Java API
 * Ruby API

Future planned features:
 * Generation V-VI save support
 * Pokémon Showdown import/export
 * QR code import/export

Build Dependencies
-------------------------------------

* C/C++
  * Core Library
    * [CMake](https://www.cmake.org) (2.8.9+)
    * [Boost](https://www.boost.org) (1.55+)
    * [Python](https://www.python.org) (2.7+ or 3.3+)
  * Qt Widgets:
    * [Qt](https://www.qt.io) (4/5)
* SWIG wrappers
  * Common dependencies
    * [SWIG](http://swig.org/)
      * Lua support: 2.0.12+
      * Python 2 support: 3.0.8+
      * Ruby support: 3.0.3+
      * Others: 2.0.11+
    * [CppHeaderParser](https://pypi.python.org/pypi/CppHeaderParser/)
    * [PLY](https://pypi.python.org/pypi/ply)
  * C# ([.NET](https://www.microsoft.com/net)/[Mono](http://www.mono-project.com/))
  * [Lua](https://www.lua.org) (5.1+)
  * [Python](https://www.python.org) (2.7+ or 3.3+)
* Documentation
  * [Doxygen](https://www.stack.nl/~dimitri/doxygen/)

Testing
-------------------------------------

Using [Travis](https://travis-ci.org/ncorgan/libpkmn) and
[AppVeyor](https://ci.appveyor.com/project/ncorgan/libpkmn), every push to this
repository is automatically tested under the following configurations:

 * Ubuntu 14.04 (x64)
   * C/C++
     * GCC: 4.8.4, 6.2.0
     * Clang: 3.5.0, 4.0.0
     * Boost: 1.55.0, 1.60.0
     * Qt: 4.8.6, 5.2.1
   * SWIG: 2.0.11, 3.0.2
   * C#: Mono 3.2.8
   * Lua: 5.1.5
   * Python: 2.7.6, 3.4.3
   * Ruby: 2.3.0
 * Windows Server 2012 R2 (x64)
   * C/C++: MSVC 2015 (Win32, x64)
     * Boost: 1.59.0, 1.62.0, 1.63.0
     * Qt: 5.7.1
   * SWIG: 3.0.6
   * C#: .NET 3.5
   * Lua: 5.1.5, 5.2.4, 5.3.2
   * Python: 3.5.2

Tests are run with the following frameworks:
 * C++: [Google Test](https://github.com/google/googletest)
 * C: [Unity](https://github.com/ThrowTheSwitch/Unity)
 * C#: [NUnit](https://www.nunit.org/)
 * Lua: [Luaunit](https://github.com/bluebird75/luaunit)
 * Python: [unittest](https://docs.python.org/3/library/unittest.html) + [nose-parameterized](https://pypi.python.org/pypi/nose-parameterized/)

All source files are analyzed with [CppCheck](http://cppcheck.sourceforge.net/).

Acknowledgments
-------------------------------------

This project is largely possible thanks to Alex "Eevee" Munroe ([@eevee](https://github.com/eevee)) whose extensive SQLite database on all things Pokémon underlies the majority of this code's functionality.

A special thanks to Michael Bond ([@codemonkey85](https://github.com/codemonkey85)) for consulting on game mechanics.

This software is in no way affiliated with or endorsed by Nintendo, Creatures Inc. or Game Freak Inc, and is created for solely recreational and non-profit use. Pokémon © 2002- Pokémon. © 1995- Nintendo/Creatures Inc./GAME FREAK inc. TM, ® and Pokémon character names are trademarks of Nintendo.

README last updated: 2017/06/04
