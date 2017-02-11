#!/bin/bash
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

REPO_TOPLEVEL=$PWD
mkdir -p test-env
cd test-env
[ $? -ne 0 ] && exit 1

# Check source
find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]pp' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
find $REPO_TOPLEVEL/lib $REPO_TOPLEVEL/include $REPO_TOPLEVEL/testing -name '*.[ch]' | xargs cppcheck --error-exitcode=1 --force 1>/dev/null
[ $? -ne 0 ] && exit 1

export PYTHONPATH=/usr/lib/python2.7/dist-packages:/usr/lib/python2.7/site-packages:$PYTHONPATH
export PYTHONPATH=/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/site-packages:$PYTHONPATH

cmake -DCMAKE_BUILD_TYPE=Release \
  -DDESIRED_QT_VERSION=$DESIRED_QT_VERSION \
  $REPO_TOPLEVEL
[ $? -ne 0 ] && exit 1
make
[ $? -ne 0 ] && exit 1
ctest -E ".*GUI" --output-on-failure
[ $? -ne 0 ] && exit 1

echo # So we can check the last error code
