CC = g++
flags = -g -O0
libs = -lncurses
src = main.cpp Projectile.cpp EnemyShip.cpp PlayerShip.cpp
exe = Space_Raiders 

$(exe): all

all: $(files)
	$(CC) $(flag) $(src) $(libs) -o $(exe)

run: $(exe)
	./$(exe)

clean:
	rm -rf ./$(exe) *.dSYM
