LIBS = -lglut -lGLU -lGL -lfreeimage
load: image_filter.cpp
	g++ -o filter image_filter.cpp $(LIBS)
