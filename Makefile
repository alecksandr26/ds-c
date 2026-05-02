C = cc

C_DEBUG_FLAGS = -Wall -Wextra -pedantic -ggdb -fPIC
C_COMPILE_FLAGS = -O2 -DNDEBUG -fno-stack-protector -z execstack -no-pie -fPIC
C_EXTERN_LIBS_FLAGS = -lexcept -lmem
C_FLAGS = $(C_GP_FLAGS) $(C_DEBUG_FLAGS)

AR = ar rc

BUILD_DIR = build
OBJ_DIR = $(addprefix $(BUILD_DIR)/, obj)
LIB_DIR = $(addprefix $(BUILD_DIR)/, lib)
TEST_DIR = $(addprefix $(BUILD_DIR)/, test)
OBJS = $(addprefix $(OBJ_DIR)/, vec.o ds_mem.o ds_exception.o linked.o iterator.o)
STATIC_LIB = ds.a
LIBS = $(addprefix $(LIB_DIR)/, ds.so $(STATIC_LIB))
TESTS = $(addprefix $(TEST_DIR)/, test_ds_mem.out test_vec.out test_linked.out)

SRC_DIR = src
INCLUDE_DIR = include

.PHONY: all, run, clean, compile, test, profile, res
all: $(TESTS)

$(BUILD_DIR):
	mkdir -p $@

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(C) $(C_FLAGS) -c $< -o $@ $(C_LIBS_FLAGS)

$(LIB_DIR): $(BUILD_DIR)
	mkdir -p $@

$(TEST_DIR): $(BUILD_DIR)
	mkdir -p $@

$(LIB_DIR)/%.a: $(OBJS) | $(LIB_DIR)
	$(AR) $@ $^
	ranlib $@

$(LIB_DIR)/%.so: $(OBJS) | $(LIB_DIR)
	$(C) -shared -o $@ $(C_FLAGS_WHOLE_ARCHIVE) $^ $(C_LIBS_FLAGS) $(C_FLAGS_NO_WHOLE_ARCHIVE)

$(TEST_DIR)/%.out: $(SRC_DIR)/%.c | $(LIBS) $(TEST_DIR)
	$(C) $(C_FLAGS) $^ $(LIB_DIR)/$(STATIC_LIB) -o $@ $(C_EXTERN_LIBS_FLAGS)

%.out: $(TEST_DIR)/%.out
	./$(TEST_DIR)/$@

test: $(notdir $(TESTS))

clean:
	rm -r -v $(BUILD_DIR)

