#!/bin/bash

FIFOFILE_FOR_CMD=/tmp/fifo_cmd
FIFOFILE_ANSWER=/tmp/$$
SERVER_EXE=./server

MY_PID=$$

function check_server_running {
	case "$(pidof $SERVER_EXE | wc -w)" in
	0) echo "Start server first. Exit"
		exit
#		./$SERVER_EXE $FIFOFILE_FOR_CMD &
	esac
}

check_server_running

# Открыть $FIFOFILE_FOR_CMD как дескриптор с номером $PIPE_FD,
# чтобы echo при выходе не закрывала канал, тем самым посылая EOF
PIPE_FD=3
#exec ${PIPE_FD}> $FIFOFILE_FOR_CMD
exec 3> $FIFOFILE_FOR_CMD

trap 'ctrl_c_handler' SIGINT

function ctrl_c_handler {
	echo "Remove answer fifo [$FIFOFILE_ANSWER]"

	# Закрыть дескриптор
	#exec 3<$-

	rm $FIFOFILE_ANSWER
	if [ $? != 0 ]; then
		echo "rm failed. Error code = $?"
		exit -1
	fi

	exit 0
}

# Канал для приёма ответа
mkfifo $FIFOFILE_ANSWER

if [ $? != 0 ]; then
	echo "Error creating pipe [$FIFOFILE_FOR_CMD]. Error code = $?"
	exit -1
fi

while :;
do
	echo -n "R: "

	read req
	if [ $? != 0 ]; then
		echo "Read failed. Error code = $?"
		exit -1
	fi

	#echo "Request is $req"
	#echo -n "sending... "

	# Посылаем запрос
	#TODO: Ctrl-C не убивает echo: http://mywiki.wooledge.org/SignalTrap
	echo "$$ $req" > $FIFOFILE_FOR_CMD

	# Принимаем ответ
	echo -n "A: "
	cat $FIFOFILE_ANSWER
	echo ""
done
#rm $FIFOFILE_FOR_CMD

exit 0
