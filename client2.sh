#!/bin/bash

SERVER_EXE=./server

function check_server_running {
	case "$(pidof $SERVER_EXE | wc -w)" in
	0) echo "Start server first. Exit"
		exit
	esac
}

check_server_running

# Добавить сюда новую команду
# Одна строка - одна команда с аргументами
# "команда" "-аргумент1" ... "-аргументN"
arr=( 
"get-led-color" 
"set-led-color" "-green" "-blue" "-red"
"get-led-state"
"set-led-state" "-on" "-off"
"get-led-rate"
"set-led-rate" "-0..5"
)

#i=0
#while [ $i -lt ${#req_array[@]} ]; do
#	echo "$i: ${req_array[$i]}"
#	((i++))
#done

source protocol2.sh

init_client

while :;
do
	set_request
	get_answer
done

exit 0
