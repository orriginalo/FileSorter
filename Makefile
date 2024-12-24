app: main.cpp
	g++ ./main.cpp -o ./main
	./main.exe



.PHONY: clean

clean:
	del ./main.exe