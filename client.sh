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
req_array=( 
"get-led-color" 
"set-led-color" 
"get-led-state"
"set-led-state"
"get-led-rate"
"set-led-rate"
)

#i=0
#while [ $i -lt ${#req_array[@]} ]; do
#	echo "$i: ${req_array[$i]}"
#	((i++))
#done

source protocol.sh

init_client

while :;
do
	set_request
	get_answer
done

exit 0
