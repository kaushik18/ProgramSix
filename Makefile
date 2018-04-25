# Name: Kaushik Nadimpalli
# kxn160430
# CS3377.502
# kxn160430@utdallas.edu
# Program 6


# Below are the flags necessary for compilation of C++ 
# We must ensure program runs with the --Wall flag so it was included
# Link against Dr. Perkins' copy of CDK Library
# We got the files example and bin file from Dr.Perkins' folder in scratch directory.

CXX = g++
CXXFLAGS = -Wall -g 
CPPFLAGS = -I/scratch/perkins/include
LDFLAGS = -L/scratch/perkins/lib
LDLIBS = -lcdk -lcurses 

#lcdk and -lcurses are compilation flags used to link the CDK library tools



# PROJECTNAME is a descriptive name used for the backup target
# This should not contain spaces or special characters

EXECFILE = cdkexample

OBJS = cdkexample.o


all: $(EXECFILE)

# Cleans the object files and only keeps the necessary files to create executable
clean:
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#*


$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# Backup Target: For safety purposes                                                                                              
backup: clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H.%M.%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!
