#!/bin/sh

FILE=mytar
if ! test -x "$FILE"; then
    echo "$FILE no existe o no se puede ejecutar."
    exit 0
fi

FILE=temp
if [ -d "$FILE" ]; then
    rm -r "$FILE"
fi

mkdir temp
cd temp

echo 'Hello world!' > file1.txt
head -n 10 /etc/passwd > file2.txt
head -c 1024 /dev/urandom > file3.txt

../mytar -cf filetar.mtar file1.txt file2.txt file3.txt

mkdir out
cp filetar.mtar out

cd out
../../mytar -xf filetar.mtar


diff ../file1.txt file1.txt
SAME1=$?

diff ../file2.txt file2.txt
SAME2=$?

diff ../file3.txt file3.txt
SAME3=$?


if [[ $SAME1 -eq 0 ]] && [[ $SAME2 -eq 0 ]] && [[ $SAME3 -eq 0 ]]; 
then
echo "Correct"
exit 0;
else
echo "Los archivos no son iguales"
exit 1;
fi
