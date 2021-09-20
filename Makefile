CXXFLAGS=-O2 -DNDEBUG $(OPTFLAGS)
CFLAGS=-O2 -DNDEBUG $(OPTFLAGS)
LIBS=$(OPTLIBS)
PREFIX?=/usr/local
#CC=g++
CC=gcc

SOURCES=src/blast.c src/isextract.c src/main.c
OBJECTS=src/blast.o src/isextract.o src/main.o

TEST_SRC=$(wildcard tests/*_tests.cpp)
TESTS=$(patsubst %.cpp,%,$(TEST_SRC))

EXECUTABLE=build/isextract

# The Target Build
all: $(EXECUTABLE)

dev: CXXFLAGS=-g -Wall -Wextra $(OPTFLAGS)
dev: all

win32:
	/usr/bin/make -f Makefile.win32 CC=i586-mingw32msvc-g++ \
	CXX=i586-mingw32msvc-g++

	
$(EXECUTABLE): build $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

build:
	@mkdir -p build
	@mkdir -p bin

# The Cleaner
clean:
	rm -rf $(OBJECTS) $(TESTS) $(TARGET) mtrace*.txt

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(EXECUTABLE) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true
