CXX = g++
CXXFLAGS = -O2
SRC = main.cpp
DST = FileSorter

# Определяем платформу
ifeq ($(OS),Windows_NT)
    RM = del /q
    RUN = $(DST).exe
    STRIP = strip $(DST).exe
else
    RM = rm -f
    RUN = ./$(DST)
    STRIP = strip $(DST)
endif

app: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(DST) $(SRC)
	$(STRIP)
	$(RUN)

.PHONY: clean format

format:
	clang-format -i ./*.cpp
	
clean:
	$(RM) $(DST)
