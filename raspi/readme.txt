This directory contains the following folders:
    3rdParty --> Libraries that other projects depend on. Run the buildAll script from the 3rdParty directory to build and install the libs to common
    common --> Holds the built 3rdParty libs as well as some makefile templates and some makefile defines
    dist --> Results of building the different projects are placed in the appropriate subdirectory of this folder
    simple_test --> Look at this directory as an example of how to edit the makefile
    {other directories} --> Projects to place on the RaspberryPi, use the Makefile.* as templates for building these projects
