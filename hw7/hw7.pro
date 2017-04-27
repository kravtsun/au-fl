TEMPLATE = subdirs
CONFIG += ordered
CONFIG -= core gl gui
MAKEFILE = Makefile.hw7

SUBDIRS = \
    cfg \
    cyk \

cyk.subdir = cyk
cyk.depends = cfg

cfg.subdir = ../lib/cfg

QMAKE_CLEAN += $${cyk.subdir}/Makefile $${cfg.subdir}/Makefile

