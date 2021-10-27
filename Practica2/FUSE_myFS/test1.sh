#!/bin/bash

MPOINT="./mount-point"

rm -R -f test
mkdir test

mkdir temp
cd temp

echo 'file 1' > ./FUSE_myFS/src/myFS.h
echo "Copying file 1"

echo 'file 2' > ./FUSE_myFS/src/fuseLib.h
echo "Copying file 2"


cp ./test/file1.txt $MPOINT/
read -p "Press enter..."

echo "Creating file 2"
echo 'This is file 2' > $MPOINT/file2.txt
ls $MPOINT -la
read -p "Press enter..."

