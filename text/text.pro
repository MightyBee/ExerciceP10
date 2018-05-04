QMAKE_CXXFLAGS += -std=c++11

TARGET = exerciceP10_text

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH = ../general

SOURCES += \
    main_text.cc \
    TextViewer.cc

HEADERS += \
    ../general/Systeme.h \
    ../general/constantes.h \
    textViewer.h \
    ../general/Dessinable.h \
    ../general/SupportADessin.h \
    ../general/Oscillateur.h
