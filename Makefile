CC=gcc

mssh:	mssh.c ./history/history.o ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./alias/alias.o
	gcc -g mssh.c ./history/history.o ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./utils/fileUtil_64.o ./alias/alias.o -o mssh

alias.o:	./alias/alias.c ./alias/alias.h
	gcc -g -c ./alias.c

history.o:	./history/history.c ./history/history.h
	gcc -g -c ./history/history.c

linkedList.o:	./linkedlist/linkedList.h ./linkedlist/linkedList.c ./linkedlist/genericData.h ./linkedlist/requiredIncludes.h
	gcc -c ./linkedlist/linkedList.c

listUtils.o:	./linkedlist/listUtils.c ./linkedlist/listUtils.h
	gcc -c ./linkedlist/listUtils.c

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

myUtils.o:	./utils/myUtils.o ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

process.o:	./process/process.c ./process/process.h
	gcc -g -c ./process/process.c	

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c

clean:
	rm ./pipes/pipes.o	
	rm ./utils/myUtils.o
	rm ./process/process.o
	rm ./tokenize/makeArgs.o
	rm mssh

