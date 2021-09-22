LIBS = -lglut -lGLU -lGL -lfreeimage
load: image_filter.c
	gcc -o filter image_filter.c $(LIBS)
