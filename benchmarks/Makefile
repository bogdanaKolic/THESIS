OBJS=stream

all: $(OBJS)

#stream: stream.c
#	gcc -O stream.c -o stream

clean:
	rm $(OBJS)

stream: stream.c
    gcc -fopenmp -D_OPENMP stream.c -o stream
    export OMP_NUM_THREADS=3

