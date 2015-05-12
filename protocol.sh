#!/bin/bash

FIFOFILE_FOR_CMD=/tmp/fifo_test
FIFOFILE_ANSWER=/tmp/$$

function ctrl_c_handler {
        #echo "Remove answer fifo [$FIFOFILE_ANSWER]"

        # Закрыть дескриптор
        #exec 3<$-

        rm $FIFOFILE_ANSWER
        if [ $? != 0 ]; then
                echo "rm $FIFOFILE_ANSWER failed. Error code = $?"
                exit -1
        fi

        exit 0
}

# Инициализация
function init_client()
{
	trap 'ctrl_c_handler' SIGINT

	# Открыть $FIFOFILE_FOR_CMD как дескриптор с номером $PIPE_FD,
	# чтобы echo при выходе не закрывала канал, тем самым посылая EOF
	exec 3> $FIFOFILE_FOR_CMD

	# Канал для приёма ответа
	mkfifo $FIFOFILE_ANSWER

	if [ $? != 0 ]; then
		echo "Error creating pipe [$FIFOFILE_ANSWER]. Error code = $?"
		exit -1
	fi
}

# Отправляем запрос
function set_request()
{
	while :; do
		echo -n "R: "

		read -r req
		if [ $? != 0 ]; then
			echo "Read failed. Error code = $?"
			exit -1
		fi

		# Проверим, не удалил ли сервер канал
		if [[ ! -e $FIFOFILE_FOR_CMD ]]; then
			echo "Server is not alive. Exit"
			exit 0
		fi

		orig_req=$req

		if [[ $req =~ \".*\\n\" ]]; then
			req=`echo $req | cut -d'"' -f 2 | cut -d'\' -f 1`

			cmd=`echo $req | cut -d' ' -f 1`
			#echo $cmd

			match=0
			i=0
			# Проверяем содержится ли команда в списке известных команд
			while [ $i -lt ${#req_array[@]} ]; do
				#echo "$i: ${req_array[$i]}"
				if [[ $cmd = ${req_array[$i]} ]]; then match=1; break; fi
				((i++))
			done

			if [[ $match -eq 0 ]]; then
				echo "Wrong command $orig_req"
				echo "Available commands are:"
				i=0
				while [ $i -lt ${#req_array[@]} ]; do
					echo -e "\t\"${req_array[$i]}\""
					((i++))
				done
				continue
			fi

			# Посылаем запрос
			# "$$" - канал для ответа
			echo "$$ $req" > $FIFOFILE_FOR_CMD
			break
		else
			echo "Error request format. Format is the following: \"get-led-cmd\n\""
		fi
	done
}

# Принимаем ответ
function get_answer()
{
        echo -n "A: "
        cat $FIFOFILE_ANSWER
        echo ""
}
