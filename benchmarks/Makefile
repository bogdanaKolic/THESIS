# as instructed at https://www.cs.virginia.edu/stream/ref.html

OBJS=stream

all: $(OBJS)

stream: stream.c
	gcc -O stream.c -o stream

clean:
	rm $(OBJS)

# gcc -fopenmp -D_OPENMP stream.c -o stream
# export OMP_NUM_THREADS=2