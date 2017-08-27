#-------------------------------------------------------------------------------
# mini_extractor build system
# minivideo library must be built first in order for mini_extractor to work
#-------------------------------------------------------------------------------

TARGET       = mini_extractor
TEMPLATE     = app
CONFIG      += c++11

# build artifacts
OBJECTS_DIR  = build/
MOC_DIR      = build/
RCC_DIR      = build/
UI_DIR       = build/
DESTDIR      = build/

# mini_extractor files
SOURCES     += src/main.cpp
HEADERS     += src/main.h

# minivideo library
INCLUDEPATH += ../minivideo/src
QMAKE_LIBDIR+= ../minivideo/build
LIBS        += -L../minivideo/build -lminivideo # dynamic linking
#LIBS        += ../minivideo/build/libminivideo.a # static linking