#!/bin/sh
if [ ! -n "$1" ] ;then
    echo "you have to select a PROGRAM!"
    exit
fi

make software_clean PROGRAM=$1 BOARD=sirv-e203-lichee
