CC= clang++
CFLAGS=-c -Wall -Werror -std=c++14 -O3

INCLUDE= -Iinclude \
	-I/System/Library/Frameworks/OpenGL.framework/Headers \
	-Ithird-party/glm

LDFLAGS= -L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
TEST_LDFLAGS= $(LDFLAGS) -L.

LIBS= -lGL
TEST_LIBS= $(LIBS) \
	-lglfw3 \
	-framework Cocoa \
	-framework CoreVideo \
	-framework IOKit \
	-lugly

SOURCES= \
	src/buffer.cpp \
	src/context.cpp \
	src/controller.cpp \
	src/framebuffer.cpp \
	src/generated_object.cpp \
	src/pipeline.cpp \
	src/program.cpp \
	src/query.cpp \
	src/renderbuffer.cpp \
	src/sampler.cpp \
	src/shader.cpp \
	src/texture.cpp \
	src/transform_feedback.cpp \
	src/uniform.cpp \
	src/vertex_array.cpp \
	src/viewport.cpp


TEST_SOURCES= \
	test/main.cpp

OBJECTS= $(SOURCES:.cpp=.o)
TEST_OBJECTS= $(TEST_SOURCES:.cpp=.o)

OUTPUT_SO=libugly.dylib

lib: $(OBJECTS)
	$(CC) -dynamiclib -Wl,-undefined -Wl,dynamic_lookup $(OBJECTS) -o $(OUTPUT_SO)

test: lib $(TEST_OBJECTS)
	$(CC) $(TEST_LDFLAGS) $(TEST_LIBS) $(TEST_OBJECTS) -o main

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJECTS) $(OUTPUT_SO)
	rm -f $(TEST_OBJECTS) main
