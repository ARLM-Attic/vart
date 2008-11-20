#V-ART Linux Makefile Template

APPLICATION=abuilder
CXXFLAGS = -Wall -I. -I/usr/include/xercesc -I/usr/include/xercesc/dom -DVART_OGL -DVISUAL_JOINTS
#CXXFLAGS = -Wall -I. -I/usr/include/xercesc -I/usr/include/xercesc/dom -DVART_OGL
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXi -lxerces-c -lpthread

OBJECTS = point4d.o color.o light.o texture.o material.o boundingbox.o memoryobj.o\
dot.o sgpath.o scenenode.o graphicobj.o sphere.o cylinder.o uniaxialjoint.o\
biaxialjoint.o polyaxialjoint.o mesh.o transform.o bezier.o modifier.o dof.o\
joint.o time.o sineinterpolator.o dofmover.o noisydofmover.o jointmover.o\
baseaction.o jointaction.o xmlbase.o xmljointaction.o action.o\
picknamelocator.o linearinterpolator.o rangesineinterpolator.o\
snlocator.o addresslocator.o descriptionlocator.o camera.o file.o meshobject.o\
scene.o xmlscene.o offsetmodifier.o\
pnmexporter.o human.o mousecontrol.o viewerglutogl.o main.o

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
