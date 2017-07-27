CC := g++ # This is the main compiler
# # CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/raytracer

TESTER := 0

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS := -std=c++11 -g -Wall
LIB := -L lib -lGL -lglut
INC := -I include

TESTDIR := test
TESTLIB := -L lib -L/home/gc/Work/google-test/googletest/googletest -lgtest -lpthread -lGL -lglut 
TESTINC := -I include -I/home/gc/Work/google-test/googletest/googletest/include
TESTSOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
TESTOBJECTS := $(patsubst $(TESTDIR)/%, $(TESTDIR)/%, $(TESTSOURCES:.$(SRCEXT)=.o))


$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) -o $(TARGET) $^  $(LIB)"; $(CC) -o $(TARGET) $^ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@if [ $(TESTER) = 1 ]; then\
		$(CC) -D TESTER  $(CFLAGS) $(TESTINC) -c -o $@ $<; \
	else\
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<; \
	fi

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

#Tests
tester:TESTER = 1
tester: $(TESTOBJECTS) $(OBJECTS) 
	@echo $(TESTER)
	$(CC) $(TESTOBJECTS) $(OBJECTS) $(CFLAGS) $(TESTINC) $(TESTLIB) -o bin/tester

$(TESTDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	$(CC) -D TESTER  $(CFLAGS) $(TESTINC) -c -o $@ $<


#   # Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
