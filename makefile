#------------------------------------------------------------------------------
# Prepare file groups
#------------------------------------------------------------------------------

# Directories
SOURCE_DIR			:= source/
LIB_SOURCE_DIR		:= lib/source/
INCLUDE_DIR			:= include/
LIB_INCLUDE_DIR		:= lib/include/
OBJECT_DIR			:= object/

TEST_DIR			:= test/
TEST_SOURCE_DIR		:= test/source/
TEST_INCLUDE_DIR	:= test/include/

# Files
COMMON_SOURCE	:= $(shell find $(LIB_SOURCE_DIR) -name "*.c") $(shell find $(TEST_SOURCE_DIR) -name "common.c") $(shell find $(SOURCE_DIR) -name "*.c")
COMMON_OBJECT	:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(COMMON_SOURCE))))

TEST_HASH_FUNCTIONS_SOURCE	:= $(TEST_SOURCE_DIR)/test_hash_function.c
TEST_HASH_FUNCTIONS_OBJECT 	:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(TEST_HASH_FUNCTIONS_SOURCE)))) $(COMMON_OBJECT)

TEST_HASH_FUNCTIONS_DEP		:= $(patsubst %.o,%.o.d, $(TEST_HASH_FUNCTIONS_OBJECT))

# Executable
TEST_HASH_FUNCTIONS	:= test_hash_function
TEST_HASH_TABLE		:= test_hash_table

# Compilation
CC			:= gcc
FLAGS		:= -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code
SANITIZE	:= -fsanitize=address -fsanitize=undefined -fsanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=null -fsanitize=alignment
INCLUDE		:= -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR) -I$(TEST_INCLUDE_DIR)

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# Compile files
#------------------------------------------------------------------------------

# Compile test_hash_function file
$(TEST_HASH_FUNCTIONS): $(OBJECT_DIR) $(TEST_HASH_FUNCTIONS_OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(TEST_HASH_FUNCTIONS_OBJECT) -o $@

# Compile test_hash_table file
# $(TEST_HASH_TABLE): $(OBJECT_DIR) $(OBJECT)
# 	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(OBJECT) -o $@

# Include dependencies
-include $(TEST_HASH_FUNCTIONS_DEP)

# Make object files
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

$(OBJECT_DIR)%.o: $(LIB_SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

$(OBJECT_DIR)%.o: $(TEST_SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

# Make object directory
$(OBJECT_DIR):
	@mkdir -p $@

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
