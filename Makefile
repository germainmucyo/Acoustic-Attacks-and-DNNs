CC = gcc
LDPATH = -L/home/germainmucyo/lab-5-germainmucyo/openssl
CFLAGS = -I/home/germainmucyo/lab-5-germainmucyo/openssl/include/ -ldl
LDLIB = -lcrypto

all: attacker victim analysis cache_benchmark

attacker: attacker.c
	$(CC) attacker.c -o attacker $(LDLIB) $(LDPATH) $(CFLAGS)

victim: victim.c
	$(CC) victim.c -o victim $(LDLIB) $(LDPATH) $(CFLAGS)

analysis: analysis.c
	$(CC) analysis.c -o analysis

cache_benchmark: cache_benchmark.c
	$(CC) cache_benchmark.c -o cache_benchmark -lrt

clean:
	rm -rf victim attacker analysis cache_benchmark
