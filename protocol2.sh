#!/bin/bash

FIFOFILE_FOR_CMD=/tmp/fifo_test
FIFOFILE_ANSWER=/tmp/$$

IFS=''

INVITATION="R: \""
SECOND_PAR="-"

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
while :;
do
	req=
	recF=
	g_match=0

	echo -n "${INVITATION}"

	while :;
	do
		# Читаем по одному символу и отображаем его потом сами
		read -r -s -n1 c

		# Проверим, не удалил ли сервер канал
		if [[ ! -e $FIFOFILE_FOR_CMD ]]; then
			echo -e "\nServer is not alive. Exit"
			exit 0
		fi

		len=${#req}
		# "Tab" symbol
		if [[ $c = `echo -e '\t'` ]]; then
			if [[ $len -ne 0 ]]; then
				match=0
				hint_str=
				i=0
				for hint in "${arr[@]}"
				do
					((i++))
					#if [[ ${hint:0:len} = $req ]]; then

					# Есть частичное(или полное) сопадение
					if [[ ${hint:0:len} = $req ]]; then
						hint_str=${hint_str}$hint" "
						((match++))

						# Ищем аргументы команды по второму нажатию на tab
						if [[ $g_match -eq 1 ]]; then
							match=0
							hint_str=

							# В массиве следующие элементы после команды - 
							# её аргументы (если есть)
							while [[ $i -le ${#arr[@]} ]]; do
								hintS=${arr[$i]}

								# Нашли аргумент
								if [[ ${hintS:0:1} = ${SECOND_PAR} ]]; then
									hint_str=${hint_str}${hintS:1:${#hintS}}" "
									((i++))
									((match++))
								else
									break
								fi
							done
							break
						fi
					fi
				done

				# Нашли совпадение
				if [[ $match -eq 1 ]]; then
					hint_str=${hint_str:0:${#hint_str}-1}
					# Печатаем дополненную команду
					if [[ $g_match -eq 0 ]]; then
						echo -e -n "\n\r${INVITATION}${hint_str}"
						req=${hint_str}
						reqF=$req
						g_match=1
					else
						# Печатаем аргументы
						echo -e -n "\n${hint_str}\n${INVITATION}${req}"
					fi
				fi
				# Несколько совпадений 
				if [[ $match -gt 1 ]]; then echo -e -n "\n${hint_str}\n${INVITATION}${req}"; fi
			fi
		else
			# Обрабатываем "backspace"
			if [[ $c = $'\177' ]]; then
				l=${#req}
				if [[ $l -gt 0 ]]; then
					echo -e -n '\b \b'
					req=${req:0:$l-1}
					if [[ ${#req} -lt ${#reqF} ]]; then g_match=0; fi
				fi
				continue
			fi

			# Обрабатываем "newline". Ввод запроса завершён
			if [[ $c = `echo -e '\n'` ]]; then
				# Добавляем '"' в начало
				req=\"${req}
				break
			fi

			req=${req}$c
			echo -n -e "\r${INVITATION}"
			# Не совмещаем, чтобы отобразился "\n"
			echo -n "${req}"
		fi
	done

	orig_req=$req

	if [[ $req =~ \".*\\n\" ]]; then
		req=`echo $req | cut -d'"' -f 2 | cut -d'\' -f 1`
		cmd=`echo $req | cut -d' ' -f 1`

		match=0
		i=0
		# Проверяем содержится ли команда в списке известных команд
		while [ $i -lt ${#arr[@]} ]; do
			#echo "$i: ${arr[$i]}"
			if [[ $cmd = ${arr[$i]} ]]; then match=1; break; fi
			((i++))
		done

		if [[ $match -eq 0 ]]; then
			echo "Wrong command $orig_req"
			echo "Available commands are:"
			i=0
			while [ $i -lt ${#arr[@]} ]; do
				echo -e "\t\"${arr[$i]}\""
				((i++))
			done
			continue
		fi

		# Посылаем запрос
		# "$$" - канал для ответа
		echo "$$ $req" > $FIFOFILE_FOR_CMD
		break
	else
		echo -e "\nError request format. Format is the following: \"get-led-cmd\n\""
	fi
done
}

# Принимаем ответ
function get_answer()
{
        echo -n "A: "
        cat $FIFOFILE_ANSWER
        echo -e -n "\n"
}
