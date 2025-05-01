BALDC_RELEASE ?= 0
BALDC_LTO_ON_RELEASE ?= 1
BALDC_FSANITIZE_ON_DEBUG ?= 0

CC ?= gcc
STRIP ?= strip

CFLAGS := -Wall -Wextra -Wpedantic -MMD -MP

SRC_DIR := src
BUILD_DIR := build
BIN := baldc

TEST_DIR := test
TEST_TOOLS_DIR := $(TEST_DIR)/tools
TEST_BUILD_DIR := $(BUILD_DIR)/test
TEST_TOOLS_BUILD_DIR := $(TEST_BUILD_DIR)/tools

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
OBJ_FILES_NO_MAIN := $(filter-out $(BUILD_DIR)/main.o, $(OBJ_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_EXECS := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%,$(TEST_FILES))
TEST_OBJ_FILES := $(TEST_EXECS:=.o)
TEST_EXECUTOR := $(TEST_TOOLS_BUILD_DIR)/test_executor
TEST_EXECUTOR_DEP_FILE := $(TEST_EXECUTOR).d
TEST_DEP_FILES := $(TEST_EXECS:=.d) $(TEST_EXECUTOR_DEP_FILE)

ifeq ($(BALDC_RELEASE),1)
	CFLAGS += -DBALDC_RELEASE
	CFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -O2
	ifeq ($(BALDC_LTO_ON_RELEASE),1)
		CFLAGS += -flto
		LDFLAGS += -flto
	endif

	POST_BUILD := $(STRIP) $(BUILD_DIR)/$(BIN)
else
	CFLAGS += -g -O0
	ifeq ($(BALDC_FSANITIZE_ON_DEBUG),1)
 	    CFLAGS += -fsanitize=address,undefined,leak
	    LDFLAGS += -fsanitize=address,undefined,leak
	endif
endif

all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(BIN): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@
	$(POST_BUILD)

-include $(DEP_FILES)

.PHONY: clean tests test

clean:
	rm -rf $(BUILD_DIR)

.PRECIOUS: $(TEST_OBJ_FILES)

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_TOOLS_BUILD_DIR):
	mkdir -p $(TEST_TOOLS_BUILD_DIR)

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(TEST_BUILD_DIR)/%: $(TEST_BUILD_DIR)/%.o $(OBJ_FILES_NO_MAIN)
	$(CC) $(LDFLAGS) $^ -o $@

$(TEST_EXECUTOR).o: $(TEST_DIR)/tools/test_executor.c | $(TEST_TOOLS_BUILD_DIR)
	$(CC) -c $< -o $@

$(TEST_EXECUTOR): $(TEST_EXECUTOR).o
	$(CC) $< -o $@

tests: $(TEST_EXECS) $(TEST_EXECUTOR)

test: tests
	$(TEST_EXECUTOR) $(TEST_EXECS)

-include $(TEST_DEP_FILES)
