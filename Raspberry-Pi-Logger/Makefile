all: ias-raspberry

ias-raspberry: ias-raspberry.o pi_dht_read.o pi_mmio.o common_dht_read.o
	gcc common_dht_read.o pi_mmio.o pi_dht_read.o ias-raspberry.o -lbcm2835 -lrt -o ias-raspberry

ias-raspberry.o: ias-raspberry.c
	gcc -c ias-raspberry.c

pi_dht_read.o: source/Raspberry_Pi/pi_dht_read.c
	gcc -std=c99 -c source/Raspberry_Pi/pi_dht_read.c

pi_mmio.o: source/Raspberry_Pi/pi_mmio.c
	gcc -std=c99 -c source/Raspberry_Pi/pi_mmio.c

common_dht_read.o: source/common_dht_read.c
	gcc -c source/common_dht_read.c
clean:
	rm -rf *o ias-raspberry.o
