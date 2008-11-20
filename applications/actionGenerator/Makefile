# Linux Makefile for VPAT Example mao
# (C) Andreia Schneider 13/06/2005

CXXFLAGS = -I/usr/include/xercesc -I/usr/include/xercesc/dom -I. -DVART_OGL
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lglui -lXi
LLDLIBS = -lxerces-c -lpthread
CPPFLAGS = -Wall

OBJECTS = xmlscene.o color.o scenenode.o scene.o material.o\
boundingbox.o memoryobj.o graphicobj.o cylinder.o light.o bezier.o\
point4d.o transform.o sphere.o camera.o mousecontrol.o texture.o\
modifier.o dof.o joint.o uniaxialjoint.o biaxialjoint.o polyaxialjoint.o \
meshobject.o mesh.o jointtime.o doftime.o sgpath.o main.o

%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

generator: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LLDLIBS) $^ -o $@

clean:
	rm -f *.o *~ generator
