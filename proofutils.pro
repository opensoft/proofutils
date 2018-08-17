TEMPLATE = subdirs

network-lpr.file = network-lprprinter.pro
network-mis.file = network-mis.pro
network-ums.file = network-ums.pro
utils.file = utils.pro
utils.depends = network-lpr

SUBDIRS = utils network-lpr network-mis network-ums
