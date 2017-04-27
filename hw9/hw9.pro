TEMPLATE = subdirs
CONFIG -= core gl gui
MAKEFILE = Makefile.hw9

SUBDIRS = \
    cfg \
    genome \

genome.subdir = genome
genome.depends = cfg

cfg.subdir = ../lib/cfg

QMAKE_CLEAN += $${genome.subdir}/Makefile $${cfg.subdir}/Makefile

