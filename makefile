SHELL := /bin/zsh

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



#------------------------------------------------------------------------------
# Run tests
#------------------------------------------------------------------------------

OUTPUT_DIR	:= output/

# Rum params
TEXT		:= text.txt
HT_SIZE		:= 2000

# Makeplot script
PY			:= python3
SCRIPT		:= $(TEST_DIR)/makeplot.py
IMG_DIR		:= img/

# Hash functions output file names
HASH_FUNCTION_ZERO			:= zero_index.out
HASH_FUNCTION_FIRST_ASCII	:= first_ascii.out
HASH_FUNCTION_STRING_LEN	:= word_length.out
HASH_FUNCTION_SUM_ASCII		:= ascii_sum.out
HASH_FUNCTION_DJB2			:= djb2.out
HASH_FUNCTION_CRC32			:= crc32.out

HF_NAMES := $(HASH_FUNCTION_ZERO)		\
			$(HASH_FUNCTION_FIRST_ASCII)\
			$(HASH_FUNCTION_STRING_LEN)	\
			$(HASH_FUNCTION_SUM_ASCII)	\
			$(HASH_FUNCTION_DJB2)		\
			$(HASH_FUNCTION_CRC32)


$(OUTPUT_DIR):
	@mkdir -p $@

run_functions_test: $(OUTPUT_DIR) $(TEST_HASH_FUNCTIONS)
	@index=0; for i in $(HF_NAMES); do 											\
		./$(TEST_HASH_FUNCTIONS) $(TEXT) $(HT_SIZE) $$index > $(OUTPUT_DIR)$$i;	\
		$(PY) $(SCRIPT) $(IMG_DIR) $(OUTPUT_DIR)$$i;							\
		((index=$$index + 1));													\
	done

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
