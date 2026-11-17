# Toy integer calculator example for the ctesting library.
#
#   make            build the calculator cli
#   make test       build the calculator tests
#   make clean      remove the generated files
#
# The ctesting library is built automatically when needed.

ROOT		:=	../..
BUILD_DIR	:=	build
CC			?=	cc
CFLAGS		?=	-Wall -Wextra -Werror -Wmissing-prototypes -std=c99
CPPFLAGS	+=	-I$(ROOT)/include
LIB			:=	$(ROOT)/libsimple_tests.a

CLI			:=	calculator
TEST		:=	test_$(CLI)

CLI_OBJS	:=	$(BUILD_DIR)/calculator.o $(BUILD_DIR)/main.o
TEST_OBJS	:=	$(BUILD_DIR)/calculator.o $(BUILD_DIR)/test_calculator.o

all: $(CLI)
test: $(TEST)

$(LIB):
	$(MAKE) -C $(ROOT) $(notdir $(LIB))

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.c calculator.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(CLI): $(CLI_OBJS) $(LIB)
	$(CC) $(CFLAGS) $(CLI_OBJS) -o $@ -L$(ROOT) -lsimple_tests

$(TEST): $(TEST_OBJS) $(LIB)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $@ -L$(ROOT) -lsimple_tests

clean:
	rm -rf $(BUILD_DIR) $(CLI) $(TEST)

.PHONY: all clean test
