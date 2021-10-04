LIBS = -lglut -lGLU -lGL -lfreeimage
load: image_filter.cpp
	g++ -fopenmp -o filter image_filter.cpp $(LIBS)
