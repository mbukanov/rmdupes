.PHONY: all clean install uninstall
	
all:
	g++ main.cpp md5.h md5.cpp -o rmdupes

install:
	install ./rmdupes /usr/local/bin

uninstall:
	rm -rf /usr/local/bin/rmdupes

clean:
	rm rmdupes