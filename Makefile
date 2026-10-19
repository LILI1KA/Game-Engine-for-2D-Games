CXX ?= g++
CXXFLAGS ?= -std=c++14 -Wall -Wextra -MMD -MP -pthread
LDLIBS ?= -lncurses -pthread

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

ENGINE_NAMES := border clock collide controller cursekeyboard model movement object observer status subject view
ENGINE_OBJECTS := $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(ENGINE_NAMES)))

BREAKOUT_OBJECTS := $(ENGINE_OBJECTS) $(BUILD_DIR)/break.o $(BUILD_DIR)/break_client.o
SHOOTER_OBJECTS := $(ENGINE_OBJECTS) $(BUILD_DIR)/arlg.o $(BUILD_DIR)/arlg_client.o
ALL_OBJECTS := $(sort $(BREAKOUT_OBJECTS) $(SHOOTER_OBJECTS))
DEPENDS := $(ALL_OBJECTS:.o=.d)

.PHONY: all breakout shooter_rpg clean

all: breakout shooter_rpg

breakout: $(BIN_DIR)/breakout

shooter_rpg: $(BIN_DIR)/shooter_rpg

$(BIN_DIR)/breakout: $(BREAKOUT_OBJECTS) | $(BIN_DIR)
	$(CXX) $(BREAKOUT_OBJECTS) -o $@ $(LDLIBS)

$(BIN_DIR)/shooter_rpg: $(SHOOTER_OBJECTS) | $(BIN_DIR)
	$(CXX) $(SHOOTER_OBJECTS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPENDS)

