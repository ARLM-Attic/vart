# Makefile for VPAT
# (C) Bruno de Oliveira Schneider - SET/09/2004

# This Makefile has been created for Linux new targets for other
# platforms should be created later.

# Search the file for "FM:" for "Fix Me" stuff.

# Changelog
# September, 9/2004 - Bruno de Oliveira Schneider
# - Makefile created

# 1. CONFIGURATION VARIABLES
# 1.1 VPAT version
# FM: In the future, version number should come from a version control system,
# or from a file such as config.h .
VERSION = 1.0 

# 1.2 Names of the VPAT files
FILES = vpbezier.cpp vpbiaxialjoint.cpp vpbody.cpp vpboundingbox.cpp\
vpcolor.cpp vpcurve.cpp vpcuttools.cpp vpcutvolume.cpp\
vpcylinder.cpp vpdirectionallight.cpp vpdof.cpp vpedge.cpp vpface.cpp\
vpgraphicobj.cpp vpimage.cpp vpjoint.cpp vpjointmotion.cpp vplight.cpp\
vpmatrix.cpp vpmesh.cpp vpmodifier.cpp vpmovement.cpp vpplane.cpp\
vpplanejoint.cpp vppoint2d.cpp vppoint3d.cpp vppointlight.cpp\
vppolyaxialjoint.cpp vpraycasting.cpp vpscene.cpp vpslice.cpp vpsphere.cpp\
vpspotlight.cpp vptable.cpp vpuniaxialjoint.cpp vpvector3d.cpp vpvertex3d.cpp\
vpview.cpp vpvolume.cpp vpcamera.cpp
#vpcutplane.cpp 

# 1.3 Names of the VPAT object files to be created
OBJECTS = vpbezier.o vpbiaxialjoint.o vpbody.o vpboundingbox.o\
vpcolor.o vpcurve.o vpcuttools.o vpcutvolume.o\
vpcylinder.o vpdirectionallight.o vpdof.o vpedge.o vpface.o\
vpgraphicobj.o vpimage.o vpjoint.o vpjointmotion.o vplight.o\
vpmatrix.o vpmesh.o vpmodifier.o vpmovement.o vpplane.o\
vpplanejoint.o vppoint2d.o vppoint3d.o vppointlight.o\
vppolyaxialjoint.o vpraycasting.o vpscene.o vpslice.o vpsphere.o\
vpspotlight.o vptable.o vpuniaxialjoint.o vpvector3d.o vpvertex3d.o\
vpview.o vpvolume.o vpcamera.o
#vpcutplane.o 

# 2. FLAGS
CXXFLAGS = -fPIC -I../include

# 3. RULES
# Force make to check .h files when deciding whether to recompile a .o file
%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

# 4. TARGETS
help:
	@echo "Possible targets for this Makefile are:"
	@echo "so:         Create Linux dynamic library"
	@echo "docs:       Create DoxyGen documentation (no time-stamp checking)"
	@echo "clean:      Remove temporary files"

# so target: create a dynamic library
so: $(OBJECTS)
	$(CXX) -shared -o libvpat.so.$(VERSION) $(OBJECTS)

docs:
	[ ! -d doc/html ] || del doc/*
	doxygen 2>&1 | tee doxygen.out

clean:
	rm -f *.o
