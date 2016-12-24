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
    [ $? -ne 0 ] && exit 1
    brew install doxygen swig qt qt5
    [ $? -ne 0 ] && exit 1
    pip install ply CppHeaderParser
    [ $? -ne 0 ] && exit 1

else
    sudo apt-get -y update
    [ $? -ne 0 ] && exit 1
    sudo apt-get -y install build-essential cmake clang cppcheck valgrind \
	                    doxygen libpython-dev python-ply libboost1.55-all-dev \
			    swig python-pip libqt4-dev qtbase5-dev mono-complete \
			    nunit-console
    [ $? -ne 0 ] && exit 1
    sudo pip install CppHeaderParser
    [ $? -ne 0 ] && exit 1
fi

echo # So we can check the last error code
