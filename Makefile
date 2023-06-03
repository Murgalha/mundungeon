# DIRS
WRKDIR = build
SRC_DIR = src
BIN_DIR = $(WRKDIR)/bin
BIN = mundungeon

#FILES
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%.cpp=$(WRKDIR)/%.o)

# FLAGS
LDLIBS_LOCAL := -lGL -lm $(shell pkg-config --libs freetype2) $(shell sdl2-config --libs)
LDLIBS += $(LDLIBS_LOCAL)

CPPFLAGS_LOCAL := -Iinclude $(shell pkg-config --cflags freetype2) $(shell sdl2-config --cflags)
CPPFLAGS += $(CPPFLAGS_LOCAL)
CXXFLAGS += -Wall -Wextra -Werror

$(BIN_DIR)/$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(WRKDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -g -DDEBUG

.PHONY: clear

clear:
	$(RM) -r $(WRKDIR)

run: $(BIN_DIR)/$(BIN)
	@$(BIN_DIR)/$(BIN)
