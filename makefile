#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

# Directories
SOURCE_DIR		:= source/
LIB_SOURCE_DIR	:= lib/source/
INCLUDE_DIR		:= include/
LIB_INCLUDE_DIR	:= lib/include/
OBJECT_DIR		:= object/
TEST_DIR		:= test/

# Files
TEST_HASH_FUNCTIONS_SOURCE	:= $(TEST_DIR)/test_hash_functions.c
COMMON_SOURCE	:= $(shell find $(LIB_SOURCE_DIR) -name "*.c") $(shell find $(SOURCE_DIR) -name "*.c" -not -name "hash_table.c")
SOURCE			:= $(COMMON_SOURCE) $(shell find $(SOURCE_DIR) -name "hash_table.c")

COMMON_OBJECT	:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(COMMON_SOURCE))))
OBJECT			:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(SOURCE))))
TEST_HASH_FUNCTIONS_OBJECT 	:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(TEST_HASH_FUNCTIONS_SOURCE)))) $(COMMON_OBJECT)

DEP				:= $(patsubst %.o,%.o.d, $(OBJECT))

# Executable
TEST_HASH_FUNCTIONS	:= test_hash_functions
TEST_HASH_TABLE		:= test_hash_table

# Compilation
CC			:= gcc
FLAGS		:= -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code
SANITIZE	:= -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
INCLUDE		:= -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR)

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

# Compile main file
$(TEST_HASH_FUNCTIONS): $(OBJECT_DIR) $(TEST_HASH_FUNCTIONS_OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(TEST_HASH_FUNCTIONS_OBJECT) -o $@

# $(TEST_HASH_TABLE): $(OBJECT_DIR) $(OBJECT)
# 	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(OBJECT) -o $@

# Include dependencies
-include $(DEP)

# Make object files
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

$(OBJECT_DIR)%.o: $(LIB_SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

$(OBJECT_DIR)%.o: $(TEST_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

# Make object directory
$(OBJECT_DIR):
	@mkdir -p $@

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
