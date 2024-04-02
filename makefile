CC = clang
LD = clang

CFLAGS = -Isrc -Wall -c -mfpmath=sse -fdeclspec -g -O3

LIBRARIES = -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
EXT = ".exe"
LFLAGS = -shared -Wall -g 

# The build target 
TARGET = matilda.dll
COPY = xcopy /Y
MOVE = move

EFLAGS = -L./ -lmatilda -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32

SRCDIR = src
EXMDIR = examples
OBJDIR = objects
BINDIR = bin
 
OBJ_FILES = ${OBJDIR}/matilda.o 

EXAMPLES = 1-helloWorld${EXT}

all: matilda examples

matilda: $(TARGET)
${OBJDIR}/matilda.o: ${SRCDIR}/matilda.c
	$(CC) $(CFLAGS) -o ${OBJDIR}/matilda.o ${SRCDIR}/matilda.c

$(TARGET): ${OBJ_FILES}
	$(LD) ${LFLAGS} ${LIBRARIES} ${OBJ_FILES} -o $(TARGET)
	${COPY} ${TARGET} "${BINDIR}/${TARGET}"

examples: ${EXAMPLES} matilda
${OBJDIR}/1-helloWorld.o: ${EXMDIR}/1-helloWorld.c ${EXMDIR}/helpers.h
	$(CC) $(CFLAGS) -o ${OBJDIR}/1-helloWorld.o ${EXMDIR}/1-helloWorld.c

1-helloWorld${EXT}: ${OBJDIR}/1-helloWorld.o
	$(LD) ${EFLAGS} ${OBJDIR}/1-helloWorld.o -o 1-helloWorld${EXT}
	${MOVE} 1-helloWorld${EXT} ${BINDIR}/1-helloWorld${EXT}

# llvm-objcopy
clean:
	$(RM) $(TARGET)