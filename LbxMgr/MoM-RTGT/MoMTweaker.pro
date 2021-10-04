## ---------------------------------------------------------------------------
## Copyright:   2011 Klaas van Aarsen
## License:     GPL
## Author:      I like Serena (aka Klaas van Aarsen)
## Created:     2010-05-01
## ---------------------------------------------------------------------------

include(Common.pri)

TEMPLATE = app

VERSION = 0.3.4
DEFINES += VERSION=\\\"$${VERSION}\\\"

TARGET = MoMTweaker

include(MoMTweaker.pri)

SOURCES += \
    Applications/MoMTweaker/main.cpp

DISTFILES += \
    Applications/MoMTweaker/CHANGES.txt \
    scripts/generate_code.pl
