CC = g++
LD = g++

CFLAGS = -Wall -Wno-unused -Wno-write-strings -g -O0 -std=c++11 -Wno-deprecated-declarations 
INCLUDE = -I./include -fno-omit-frame-pointer 
LIB = -L./lib  -lcrypto -lssl  -Wl,-rpath,'$$ORIGIN/../lib:$$ORIGIN/lib:$$ORIGIN'

#SRCS=$(wildcard *.c)
SRCS=$(wildcard *.cc)
#OBJS=$(patsubst %.c, %.o, $(SRCS))
OBJS=$(patsubst %.cc, %.o, $(SRCS))
HDRS = $(wildcard *.h)

TARGET = test

.PHONY:all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LIB)
 
%.o: %.cc $(HDRS)
	$(CC) -c $< $(CFLAGS) $(INCLUDE) 

clean:
	rm -f $(OBJS) $(TARGET) *.gch
