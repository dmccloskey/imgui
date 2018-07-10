# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev

CXX = g++

EXE = example_sdl_opengl2_plot.out
SOURCES = main.cpp ../imgui_impl_sdl.cpp ../imgui_impl_opengl2.cpp
SOURCES += ../../imgui.cpp ../../imgui_demo.cpp ../../imgui_draw.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

UNAME_S := $(shell uname -s)

LIBS = -lGL -ldl `sdl2-config --libs`

CXXFLAGS = -I ../ -I ../../ `sdl2-config --cflags`
CXXFLAGS += -Wall -Wformat
CFLAGS = $(CXXFLAGS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../../%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
