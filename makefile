OPENGL = -lglut -lGLU -lGL -lm
OPENCV =`pkg-config opencv --cflags` `pkg-config opencv --libs`

TARGET = test
OBJS = test.c

all: TARGET

TARGET: ${OBJS}
	gcc -o ${TARGET} -g ${TARGET}.c ${OPENGL} ${OPENCV}
