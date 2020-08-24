src=$(wildcard *.cpp)
obj=$(patsubst %.cpp, %, $(src))
CC=g++
ALL:$(obj)

%:%.cpp
	$(CC) $< -o $@

clean:
	-rm -rf $(obj)
.PHONY:clean all

