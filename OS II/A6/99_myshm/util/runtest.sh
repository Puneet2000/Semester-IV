#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib

# Syntax checking
#./shmutil

# Debugging
#set -x

#dir_prefix="/tmp"
dir_prefix=""

# --------------------------------------------------------
# Test 1 : Same process writing and reading using syscalls
# --------------------------------------------------------

shm_file=`mktemp -u ${dir_prefix}/myshm_XXX`

./shmutil --op create $shm_file
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op syswrite --op sysread --mes "test1"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 1 : Same process writing and reading using syscalls"

# --------------------------------------------------------
# Test 2 : Same process writing and reading using mapping
# --------------------------------------------------------

shm_file=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op mapwrite --op mapread --mes "test2"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 2 : Same process writing and reading using mapping"

# ----------------------------------------------------------------------------------
# Test 3 : One process writing using syscall and other process reading using syscall
# ----------------------------------------------------------------------------------

shm_file=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op syswrite --mes "test3"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op sysread --mes "test3"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 3 : One process writing using syscall and other process reading using syscall"

# ----------------------------------------------------------------------------------
# Test 4 : One process writing using mapping and other process reading using mapping
# ----------------------------------------------------------------------------------

shm_file=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op mapwrite --mes "test4"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op mapread --mes "test4"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 4 : One process writing using mapping and other process reading using mapping"

# ----------------------------------------------------------------------------------
# Test 5 : One process writing using mapping and other process reading using syscall 
# ----------------------------------------------------------------------------------

shm_file=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op mapwrite --mes "test5"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op sysread --mes "test5"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 5 : One process writing using mapping and other process reading using syscall"

# ----------------------------------------------------------------------------------
# Test 6 : One process writing using syscall and other process reading using mapping 
# ----------------------------------------------------------------------------------

shm_file=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op syswrite --mes "test6"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op mapread --mes "test6"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 6 : One process writing using syscall and other process reading using mapping"

# ----------------------------------------------------------------------------------
# Test 7 : One process writing to one shm and other process reading from another shm.
# ----------------------------------------------------------------------------------

shm_file1=`mktemp ${dir_prefix}/myshm_XXX`
shm_file2=`mktemp ${dir_prefix}/myshm_XXX`

./shmutil $shm_file1 --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file2 --op create
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file1 --op syswrite --mes "test7"
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file2 --op sysread --mes "test7" 2> /dev/null
if [ $? -eq 0 ]
then
	exit 1
fi

./shmutil $shm_file2 --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

./shmutil $shm_file1 --op delete
if [ $? -ne 0 ]
then
	exit 1
fi

rm -f $rdwr_lock

echo "PASS - Test 7 : One process writing to one shm and other process reading from another shm"

# Debugging
set +x
