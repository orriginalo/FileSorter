app: main.cpp
	g++ ./main.cpp -O2 -o ./main
	strip ./main.exe
	./main.exe


.PHONY: clean format

format:
	clang-format -i ./*.cpp
	
clean:
	del ./main.exe