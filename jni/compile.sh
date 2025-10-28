#!/bin/bash

JAVA_HOME="/usr/lib/jvm/java-17-openjdk-amd64"

SRC="lib.c"
OUT="libvirt.so" # lib name

javac Mem.java
javac -h .  Mem.java

g++ -shared -fPIC \
    -I"$JAVA_HOME/include" \
    -I"$JAVA_HOME/include/linux" \
    $SRC -o $OUT

echo "Done! Created: $OUT"

export LD_LIBRARY_PATH=$PWD
java Mem