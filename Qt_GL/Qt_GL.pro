QT             += opengl
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = exerciceP10_gl

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH = ../general

SOURCES += \
    main_qt_gl.cc \
    glwidget.cc \
    glsphere.cc \
    vue_opengl.cc

HEADERS += \
    glwidget.h \
    vertex_shader.h \
    vue_opengl.h \
    glsphere.h \
    ../general/constantes.h \
    ../general/Dessinable.h \
    ../general/Erreur.h \
    ../general/Integrateur.h \
    ../general/Oscillateur.h \
    ../general/SupportADessin.h \
    ../general/Systeme.h \
    ../general/Vecteur.h

RESOURCES += \
    resource.qrc
