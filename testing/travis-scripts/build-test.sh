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

    ls -l /usr/local/include/python*
    cmake -DPYTHON_EXECUTABLE=/usr/local/bin/python2 \
          -DPYTHON_INCLUDE_DIR=/usr/local/include/python2.7 \
          -DPYTHON_LIBRARY=/usr/local/lib/libpython2.7.dylib \
	  $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
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
    cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1

    # Clang compile check
    cd $REPO_TOPLEVEL/test-env/clang
    [ $? -ne 0 ] && exit 1
    CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Debug $REPO_TOPLEVEL
    [ $? -ne 0 ] && exit 1
    make
    [ $? -ne 0 ] && exit 1
    ctest --output-on-failure
    [ $? -ne 0 ] && exit 1
fi

echo # So we can check the last error code
