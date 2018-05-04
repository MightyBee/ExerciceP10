QMAKE_CXXFLAGS += -std=c++11

QT += gui

TEMPLATE = lib

CONFIG = staticlib

SOURCES += \
    constantes.cc \
    Dessinable.cc \
    Erreur.cc \
    Integrateur.cc \
    Oscillateur.cc \
    Systeme.cc \
    Vecteur.cc

HEADERS += \
    constantes.h \
    Dessinable.h \
    Erreur.h \
    Integrateur.h \
    Oscillateur.h \
    SupportADessin.h \
    Systeme.h \
    Vecteur.h
