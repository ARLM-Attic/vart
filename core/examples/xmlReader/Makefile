# Linux Makefile for VPAT Example mao
# (C) Andreia Schneider 13/06/2005


CXXFLAGS = -I/usr/include/xercesc -I/usr/include/xercesc/dom -I../../include -I../../../contrib/viewerglutogl -DVP_OGL
LDFLAGS = -L/usr/X11R6/lib -lGL -lglut -lGLU -lXi
LLDLIBS = -lxerces-c -lpthread
CPPFLAGS = -Wall

OBJECTS = vpxmlscene.o vpcolor.o vpscenenode.o vpscene.o vptexture.o vpmaterial.o\
vpboundingbox.o vpmemoryobj.o vpgraphicobj.o vpcylinder.o vplight.o\
vppoint4d.o\
vppoint2d.o vppoint3d.o vpvector3d.o vpvertex3d.o vpcurve.o vpbezier.o\
vptransform.o vpsphere.o vpcamera.o vpmousecontrol.o\
vpmodifier.o vpdof.o vpjoint.o vpuniaxialjoint.o vpbiaxialjoint.o vppolyaxialjoint.o vpviewerglutogl.o\
vpmeshobject.o vpmesh.o main.o

%.o: ../../source/%.cpp ../../include/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: ../../../contrib/viewerglutogl/%.cpp ../../../contrib/viewerglutogl/%.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

robotarm: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LLDLIBS) $^ -o $@

clean:
	rm -f *.o *~ robotarm
