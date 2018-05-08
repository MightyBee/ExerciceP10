TEMPLATE = subdirs

QT += gui

SUBDIRS += \
     general \
     Qt_GL \
     text

Qt_GL.depends = general

text.depends = general
