# add custom variables to this file

# OF_ROOT allows to move projects outside apps/* just set this variable to the
# absolute path to the OF root folder

OF_ROOT = ~/499/openFrameworks

# USER_CFLAGS allows to pass custom flags to the compiler
# for example search paths like:
# USER_CFLAGS = -I src/objects
USER_CFLAGS = -I /usr/include/ni
USER_CFLAGS += -I /usr/include/nite
USER_CFLAGS += -I /usr/include/openni
USER_CFLAGS += -I $(OF_ROOT)/addons/ofxOpenNI/src
USER_CFLAGS += -I $(OF_ROOT)/addons/ofxOpenNI/include/libusb 

#USER_CFLAGS += -I $(OF_ROOT)/addons/ofxOpenNI-gabby/src
#USER_CFLAGS += -I $(OF_ROOT)/addons/ofxOpenNI-gabby/include
#USER_CFLAGS += -I ~/499/include

CFLAGS += -fpermissive

# USER_LDFLAGS allows to pass custom flags to the linker
# for example libraries like:
# USER_LD_FLAGS = libs/libawesomelib.a
USER_LDFLAGS = /usr/lib/libOpenNI.so
#USER_LDFLAGS += /usr/lib/libXnDeviceSensorV2KM.so      
#USER_LDFLAGS += /usr/lib/libXnVHandGenerator_1_4_1.so
#USER_LDFLAGS += /usr/lib/libXnVCNITE_1_4_1.so          
#USER_LDFLAGS += /usr/lib/libXnVHandGenerator_1_4_2.so
#USER_LDFLAGS += /usr/lib/libXnVCNITE_1_4_2.so          
#USER_LDFLAGS += /usr/lib/libXnVHandGenerator_1_5_2.so
#USER_LDFLAGS += /usr/lib/libXnVCNITE_1_5_2.so          
#USER_LDFLAGS += /usr/lib/libXnVNite_1_3_0.so
#USER_LDFLAGS += /usr/lib/libXnVFeatures_1_3_0.so       
#USER_LDFLAGS += /usr/lib/libXnVNite_1_4_1.so
#USER_LDFLAGS += /usr/lib/libXnVFeatures_1_4_1.so       
#USER_LDFLAGS += /usr/lib/libXnVNite_1_4_2.so
#USER_LDFLAGS += /usr/lib/libXnVFeatures_1_4_2.so       
#USER_LDFLAGS += /usr/lib/libXnVNite_1_5_2.so
#USER_LDFLAGS += /usr/lib/libXnVFeatures_1_5_2.so       
#USER_LDFLAGS += /usr/lib/libXnVNITE.jni.so
#USER_LDFLAGS += /usr/lib/libXnVHandGenerator_1_3_0.so

# use this to add system libraries for example:
# USER_LIBS = -lpango
USER_LIBS = -lOpenNI

# change this to add different compiler optimizations to your project
USER_COMPILER_OPTIMIZATION = -march=native -mtune=native -Os

EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj,.git"
