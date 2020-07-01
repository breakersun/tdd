#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = My_CppUTest

#--- Inputs ----#
CPP_PLATFORM = Gcc
PROJECT_HOME_DIR = .

SRC_DIRS = \
	src/HomeAutomation \
	src/util \

TEST_SRC_DIRS = \
	tests \
	tests/HomeAutomation\
	tests/util \
	mocks \ 

	
INCLUDE_DIRS =\
  $(CPPUTEST_HOME)/include\
  include/HomeAutomation\
  include/IO\
  include/util\
  mocks \
	
CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default
CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
LD_LIBRARIES = -lpthread
	
  
ifeq ($(CPPUTEST_HOME),)
$(info CPPUTEST_HOME not set! See README.txt)
else  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
endif
