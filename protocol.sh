#!/bin/bash

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

		#echo "reg [$req]"

		if [[ $req =~ \".*\\n\" ]]; then
			# "$$" - канал для ответа
			req=`echo $req | cut -d'"' -f 2 | cut -d'\' -f 1`

			# Проверим, не удалил ли сервер канал
			if [[ ! -e $FIFOFILE_FOR_CMD ]]; then
				echo "Server is not alive. Exit"
				exit 0
			fi

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
