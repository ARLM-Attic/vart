# Linux Makefile for VPAT Example 1
# (C) Bruno de Oliveira Schneider - SET/09/2004

CXXFLAGS = -Wall -I../core/include -I../contrib -DVP_OGL -DVISUAL_JOINTS
#LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXmu -lXi
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXi

OBJECTS =  vpcolor.o vpscenenode.o vpscene.o vptexture.o vpmaterial.o\
vpboundingbox.o vpmemoryobj.o vpgraphicobj.o vpcylinder.o vplight.o\
vppoint4d.o\
vppoint2d.o vppoint3d.o vpvector3d.o vpvertex3d.o vpcurve.o vpbezier.o\
vptransform.o vpsphere.o vpcamera.o vpmousecontrol.o\
vpmodifier.o vpdof.o vpjoint.o vpbiaxialjoint.o vpviewerglutogl.o\
vigilancecamera.o main.o

%.o: ../core/source/%.cpp ../core/include/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

biaxi: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *~ biaxi
