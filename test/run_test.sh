#!/usr/bin/env sh

main_script="client.gd"
godot_executable=`command -v godot`

if [ $# -ge 1 ]; then
	if [ -f $1 ]; then
		alias godot=./$1
	else 
		echo "'$1' is not a valid file path"
		exit 1
	fi	

elif [ -z $godot_executable ]; then
	echo "godot is not found in PATH, provide a path to godot executable"
	exit 1
fi

g++ server.cpp && ./a.out & 
godot -s $main_script wait
