TARGET := raylib-demo-balls

all:
	$(CC) -lraylib ./main.c -o $(TARGET)

run: all
	./$(TARGET)
