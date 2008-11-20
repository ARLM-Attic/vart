APPLICATION=picker
CXXFLAGS = -Wall -I. -I/usr/include/xercesc -I/usr/include/xercesc/dom -DVART_OGL
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXi -lxerces-c -lpthread

OBJECTS = mesh.o memoryobj.o\
mousecontrol.o meshobject.o bezier.o modifier.o dof.o\
color.o texture.o material.o joint.o\
boundingbox.o scenenode.o camera.o transform.o\
viewerglutogl.o graphicobj.o sphere.o point4d.o\
light.o cylinder.o scene.o main.o

# first, try to compile from this project
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# second, try to compile from the core
%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

# last, try to compile from contribs
%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(APPLICATION): $(OBJECTS) main.o
	$(CXX) $(LDFLAGS) -o $@ $^

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
