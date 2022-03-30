#!/bin/bash

echo "Ruta actual: $PWD"
echo "Valor de" '$#:' $#
echo "Valor de" '$@:' $@

a=$#
b=$a*3
let c=$a*3
echo "a = "$a
echo "b = "$b
echo "c = "$c

i=0
while [$i - lt 10]
do
	echo "_______________" >> ping.txt
	host 192.100.170.$i >> ping.txt
	echo "_______________" >> ping.txt
done