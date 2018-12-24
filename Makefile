include CommonDefs.mak

BIN_DIR = bin

INC_DIRS = \
	../../Include \
	../../ThirdParty/GL/ \
	../Common

SRC_FILES = *.cpp

ifeq ("$(OSTYPE)","Darwin")
	CFLAGS += -DMACOS
	LDFLAGS += -framework OpenGL -framework GLUT
else
	CFLAGS += -DUNIX -DGLX_GLXEXT_LEGACY
	CFLAGS += -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -L/home/pengcan/projects/RobotArm/OpenNI-Linux-x64-2.3.0.43/Samples/SimpleViewer/Export_soft_d2c_v0.3.1/lib/linux
	USED_LIBS += glut GL 
	USED_LIBS += d2c opencv_dnn opencv_ml opencv_objdetect opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_calib3d opencv_features2d opencv_highgui opencv_videoio opencv_imgcodecs opencv_video opencv_photo opencv_imgproc opencv_flann opencv_core dl
endif

USED_LIBS += OpenNI2 d2c

EXE_NAME = simpleViewer

CFLAGS += -Wall



ifndef OPENNI2_INCLUDE
    $(error OPENNI2_INCLUDE is not defined. Please define it or 'source' the OpenNIDevEnvironment file from the installation)
else ifndef OPENNI2_REDIST
    $(error OPENNI2_REDIST is not defined. Please define it or 'source' the OpenNIDevEnvironment file from the installation)
endif

INC_DIRS += $(OPENNI2_INCLUDE)

include CommonCppMakefile

.PHONY: copy-redist
copy-redist:
	cp -R $(OPENNI2_REDIST)/* $(OUT_DIR)

$(OUTPUT_FILE): copy-redist









