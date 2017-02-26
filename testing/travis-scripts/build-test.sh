#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env/pkmn-build
cd test-env/pkmn-build
[ $? -ne 0 ] && exit 1

# Check source
find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]pp' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
[ $? -ne 0 ] && exit 1


if [ $PYTHON_VERSION -eq 2 ]
then
    export PYTHONPATH=/usr/lib/python2.7/dist-packages:/usr/lib/python2.7/site-packages:$PYTHONPATH
    export PYTHONPATH=/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/site-packages:$PYTHONPATH
    PYTHON_INCLUDE_DIR=/usr/include/python2.7
    PYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so
else
    export PYTHONPATH=/usr/lib/python3/dist-packages:/usr/lib/python3/site-packages:$PYTHONPATH
    export PYTHONPATH=/usr/local/lib/python3/dist-packages:/usr/local/lib/python3/site-packages:$PYTHONPATH
    PYTHON_INCLUDE_DIR=/usr/include/python3.4m
    PYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.4m.so
fi

if [ "$NEWCC" != "" ]
then
    export CC=$NEWCC
    export CXX=$NEWCXX
fi

cmake -DCMAKE_BUILD_TYPE=Release \
  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
  -DSWIG_EXECUTABLE=/usr/bin/swig${SWIG_VERSION}.0 \
  -DPYTHON_EXECUTABLE=/usr/bin/python${PYTHON_VERSION} \
  -DPYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR \
  -DPYTHON_LIBRARY=$PYTHON_LIBRARY \
  $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
ctest -E ".*GUI" --output-on-failure
[ $? -ne 0 ] && exit 1
sudo make install
[ $? -ne 0 ] && exit 1

# Test external C++ project
mkdir $REPO_TOPLEVEL/test-env/cpp-app
[ $? -ne 0 ] && exit 1
cd $REPO_TOPLEVEL/test-env/cpp-app
[ $? -ne 0 ] && exit 1
cmake $REPO_TOPLEVEL/testing/applications/cpp
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
./pkmn-cpp-app
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
