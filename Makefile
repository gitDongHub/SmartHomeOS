#create smart_home_system-arm 

CC=arm-linux-gcc
INC = ./drive/inc/
LIBPATH = ./drive/src
OBJ = smart_home_system.c $(wildcard $(LIBPATH)/*.c)
smart_home_system-arm:$(OBJ)
	$(CC) $^ -lpthread -I$(INC) -o ./bin/$@

.PHONY:clean
clean:
	rm ./bin/smart_home_system-arm
