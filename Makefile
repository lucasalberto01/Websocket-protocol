# Makefile para webcam_capture

CC = g++
CFLAGS = -Wall -std=c++11
TARGET_SERVER = server.bin
TARGET_CLIENT = client.bin
SRC_SERVER = server.cpp src/Server.cpp
SRC_CLIENT = client.cpp src/Client.cpp
INC = -I/usr/include/opencv4

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CC) $(CFLAGS) -o $(TARGET_SERVER) $(SRC_SERVER) $(INC) -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs 

$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CC) $(CFLAGS) -o $(TARGET_CLIENT) $(SRC_CLIENT) $(INC) -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs 

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)