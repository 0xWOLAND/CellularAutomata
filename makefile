CFLAGS = -g -std=c++11

LINK = g++ $(CFLAGS)

LFLAGS = -lGL -lGLEW -lglut -pthread

main: main.cpp ./headers/Shader.h ./headers/buffer.h ./headers/Objects.h ./headers/Camera.h ./headers/Mesh.h ./headers/TimerUtil.h ./shaders/shader.vs ./shaders/shader.fs
	$(LINK) main.cpp $(LFLAGS) -o main
