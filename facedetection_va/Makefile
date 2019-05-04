all:
	g++ facedetector.cpp `pkg-config opencv --cflags` -o facedetector `pkg-config opencv --libs` -lmysqlclient

clean:
	rm facedetector
