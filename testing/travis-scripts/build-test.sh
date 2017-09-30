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

# Clang builds include static analysis.
if echo "$CC" | grep clang > /dev/null
then
  find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/testing/unit-tests -name '*.[ch]pp' | xargs cppcheck --enable=performance,portability,warning --std=c++11 -I $REPO_TOPLEVEL/include --error-exitcode=1 --force --quiet
  [ $? -ne 0 ] && exit 1
  find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/testing/unit-tests -name '*.[ch]' | xargs cppcheck --enable=performance,portability,warning --std=c99 -I $REPO_TOPLEVEL/include --error-exitcode=1 --force --quiet
  [ $? -ne 0 ] && exit 1

  export USE_CLANG_TIDY=1
else
  export USE_CLANG_TIDY=0
fi

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

cmake -DCMAKE_BUILD_TYPE=Release \
  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
  -DSWIG_EXECUTABLE=/usr/bin/swig${SWIG_VERSION}.0 \
  -DPYTHON_EXECUTABLE=/usr/bin/python${PYTHON_VERSION} \
  -DPYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR \
  -DPYTHON_LIBRARY=$PYTHON_LIBRARY \
  -DPKMN_USE_CLANGTIDY=$USE_CLANGTIDY \
  $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
ctest -E ".*GUI" --output-on-failure
[ $? -ne 0 ] && exit 1
# So the log isn't too verbose
echo Installing...
sudo make install > /dev/null 2>&1
[ $? -ne 0 ] && exit 1
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig
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

# Test external C project
mkdir $REPO_TOPLEVEL/test-env/c-app
[ $? -ne 0 ] && exit 1
cd $REPO_TOPLEVEL/test-env/c-app
[ $? -ne 0 ] && exit 1
cmake $REPO_TOPLEVEL/testing/applications/c
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
./pkmn-c-app
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
