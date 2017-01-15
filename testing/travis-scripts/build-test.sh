#!/bin/bash
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env
cd test-env
[ $? -ne 0 ] && exit 1

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    # Compile test
    mkdir -p build
    cd build

    cmake -DPKMN_ENABLE_PYTHON=OFF \
	  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
	  $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest -E "cpp_QtWidgetsTest" --output-on-failure
    [ $? -ne 0 ] && exit 1
else
    # Check source
    find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]pp' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
    find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
    [ $? -ne 0 ] && exit 1

    mkdir -p gcc clang
    export PYTHONPATH=/usr/lib/python2.7/dist-packages:/usr/lib/python2.7/site-packages:$PYTHONPATH
    export PYTHONPATH=/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/site-packages:$PYTHONPATH

    # GCC compile check
    cd $REPO_TOPLEVEL/test-env/gcc
    [ $? -ne 0 ] && exit 1
    cmake -DCMAKE_BUILD_TYPE=Release \
	  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
	  $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest
    [ $? -ne 0 ] && exit 1

    # Clang compile check
    cd $REPO_TOPLEVEL/test-env/clang
    [ $? -ne 0 ] && exit 1
    CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release \
	                       -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
                               $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest
    [ $? -ne 0 ] && exit 1
fi

echo # So we can check the last error code
