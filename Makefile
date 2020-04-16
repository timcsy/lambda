lc: src/lc.cpp
	mkdir -p bin
	g++ -o bin/lc src/lc.cpp
clean:
	rm -rf bin/**/* build/**/*