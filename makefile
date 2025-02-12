CXX = g++

CXXFLAGS = -Isrc/include -Lsrc/lib -std=c++20
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC = $(wildcard *.cpp)  # 自动获取当前目录下的所有 .cpp 文件

TARGET = $(basename $(SRC))  # 生成对应的可执行文件名


all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	del $(TARGET:=.exe)