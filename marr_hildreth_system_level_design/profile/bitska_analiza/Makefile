CC = "g++"
PROJECT = bitska

SRC = bitska.cpp 

LIBS = `pkg-config opencv4 --cflags --libs`

$(PROJECT) : $(SRC)
	$(CC) -O2 $(SRC) -lsystemc -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./specification ../../data/oblici.pgm
clean:
	rm -f *.o all $(PROJECT) callgrind.*	
