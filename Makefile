CXX = g++
CXXFLAGS = -O2 -Iinclude # Указываем путь к заголовочным файлам
SRC_DIR = ./src
INCLUDE_DIR = ./include
SRC = $(wildcard $(SRC_DIR)/*.cpp) # Автоматически находим все .cpp файлы в src
DST = FileSorter

# Определяем платформу
ifeq ($(OS),Windows_NT)
    RM = del /q
    RUN = ./$(DST).exe
    STRIP = strip $(DST).exe
else
    RM = rm -f
    RUN = ./$(DST)
    STRIP = strip $(DST)
endif

app: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(DST) $(SRC)
	$(STRIP)

run: app
	$(RUN)

.PHONY: clean format

format:
	clang-format -i $(SRC_DIR)/*.cpp $(INCLUDE_DIR)/*.h
	
clean:
	$(RM) $(DST)
