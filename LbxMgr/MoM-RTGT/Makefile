## ---------------------------------------------------------------------------
## Copyright:   2011 Klaas van Aarsen
## License:     GPL
## Author:      I like Serena (aka Klaas van Aarsen)
## Created:     2010-05-01
## ---------------------------------------------------------------------------

all: generate
	cd MoMCustomizer && make all
	cd MoMTweaker && make all

check:
	cd MoMTest && make all && bin/MoMTest

generate:
	cd MoMEditorTemplate && make generate
