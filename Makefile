lc: src/lc.cpp
	mkdir -p bin
	g++ -o bin/lc src/lc.cpp
ilc: ilc.cpp
	mkdir -p bin
	g++ -o  bin/ilc src/ilc.cpp
blc: blc.cpp
	mkdir -p bin
	g++ -o  bin/blc src/blc.cpp
clean:
	rm -rf bin/**/* build/**/*