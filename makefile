#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

# Directories
SOURCE_DIR	:= source/
INCLUDE_DIR	:= include/
OBJECT_DIR	:= object/

# Files
SOURCE	:= $(shell find $(SOURCE_DIR) -name "*.c")
OBJECT	:= $(addprefix $(OBJECT_DIR),$(patsubst %.c,%.o,$(notdir $(SOURCE))))
DEP		:= $(patsubst %.o,%.o.d, $(OBJECT))

# Executable
RUN_EXE = hash_table

# Compilation
CC			:= gcc
FLAGS		:= -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code
SANITIZE	:= -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
INCLUDE		:= -I$(INCLUDE_DIR)

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

# Compile main file
$(RUN_EXE): $(OBJECT_DIR) $(OBJECT)
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) $(OBJECT) -o $@

# Include dependencies
-include $(DEP)

# Make object files
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	@$(CC) $(FLAGS) $(SANITIZE) $(INCLUDE) -MMD -MF $@.d -c -o $@ $<

# Make object directory
$(OBJECT_DIR):
	@mkdir -p $@

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
