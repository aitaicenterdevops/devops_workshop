all:
	g++ facedetector.cpp DB.cpp `pkg-config opencv --cflags` -o facedetector `pkg-config opencv --libs` -lmysqlcppconn

clean:
	rm facedetector
