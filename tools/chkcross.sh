#!/bin/sh

if [ ! -f $1/$2-gcc ]
then
	echo "Can't find cross-GCC!"
	exit 1
fi

if [ ! -f $1/$2-ld ]
then
	echo "Can't find cross-LD!"
	exit 2
fi

exit 0
