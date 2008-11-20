# Linux Makefile for VART Example of texture
# (C) Andreia Schneider 06/03/2007

CXXFLAGS = -I/usr/include/xercesc -I/usr/include/xercesc/dom -I. -DVART_OGL -DIL_LIB
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lglui -lXi -lIL
LLDLIBS = -lxerces-c -lpthread
CPPFLAGS = -Wall

OBJECTS = xmlscene.o color.o scenenode.o scene.o material.o\
boundingbox.o memoryobj.o graphicobj.o cylinder.o light.o bezier.o\
point4d.o transform.o sphere.o camera.o mousecontrol.o texture.o\
modifier.o dof.o joint.o uniaxialjoint.o biaxialjoint.o polyaxialjoint.o\
rangesineinterpolator.o sineinterpolator.o linearinterpolator.o\
meshobject.o mesh.o xmlbase.o action.o xmlaction.o jointmover.o dofmover.o sgpath.o\
noisydofmover.o time.o dot.o mousecontrol.o viewerglutogl.o main.o

%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.o: vart/source/%.cpp vart/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: vart/contrib/source/%.cpp vart/contrib/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

textureExample: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LLDLIBS) $^ -o $@

clean:
	rm -f *.o *~ textureExample
