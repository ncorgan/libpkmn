#!/bin/bash
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then

    brew update
    brew install doxygen

else
    sudo apt-get -y update
    sudo apt-get -y install build-essential cmake clang cppcheck valgrind \
	                    doxygen libpython-dev python-ply libboost-all-dev \
			    swig
fi
