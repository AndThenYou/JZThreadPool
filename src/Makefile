SRCDIR = ./

NAME = ThreadpoolDemo

OBJS = $(SRCDIR)/main.o $(SRCDIR)/JZThreadPool.o 

DIR = ../output

CC = gcc
CPP = g++ 
LD = g++ 

CFLAGS = -O3 -D__linux__ -D_LINUX -fPIC -g
DEFINES = -D_FILE_OFFSET_BITS=64
LDFLAGS = -Wl,-rpath=./

INC = -I./

LIB_LINK = -lpthread -ldl -lrt

all: exe install

exe: $(NAME)
$(NAME): $(OBJS)
	$(LD) $(OBJS) -o $(DIR)/$(NAME) $(LIB_LINK) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -c $< -o $@

%.o: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $(DEFINES) -c $< -o $@

install:

clean:
	rm -rf $(OBJS)
	rm -rf $(DIR)/$(NAME)

rebuild: clean all

