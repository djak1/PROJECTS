CC = "g++"
PROJECT = specification

SRC = proj.cpp 

LIBS = `pkg-config opencv4 --cflags --libs`

$(PROJECT) : $(SRC)
	$(CC) -g  $(SRC) -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./specification
clean:
	rm -f *.o all $(PROJECT) callgrind.*	
