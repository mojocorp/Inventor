#!/bin/sh

for i in `ls *.cpp` 
do
  dirname=`echo $i | sed 's/\.cpp$//g'`
  mkdir $dirname
  mv $i $dirname
  cd $dirname
  qmake -project
  cd ..
done


echo "TEMPLATE = subdirs" > CXX.pro
echo "SUBDIRS = " >> CXX.pro
for i in `ls`
do
  echo "$i \\" >> CXX.pro
done
