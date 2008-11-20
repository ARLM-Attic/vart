# Linux Makefile for VART Example "Vigilance Camera"
# (C) Bruno de Oliveira Schneider - SET/09/2004

CXXFLAGS = -Wall -I. -DVART_OGL -DVISUAL_JOINTS
#LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXmu -lXi
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXi

OBJECTS =  color.o scenenode.o scene.o texture.o material.o\
boundingbox.o memoryobj.o graphicobj.o cylinder.o light.o\
mesh.o meshobject.o point4d.o curve.o bezier.o\
transform.o sphere.o camera.o mousecontrol.o\
modifier.o dof.o joint.o biaxialjoint.o viewerglutogl.o\
vigilancecamera.o main.o

%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

biaxi: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *~ biaxi
