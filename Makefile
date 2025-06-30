NAME := cub3D

# Directories
OBJDIR := obj
SRCDIR := src
INCDIRS := include

# Project files
SRCS := $(addprefix $(SRCDIR)/,\
	main.c \
	cast/cast.c \
	color.c \
	input/input.c \
	input/input_funcs.c \
)

# MLX42
MLX42 := mlx42/build/libmlx42.a
INCDIRS += mlx42/include
MLX42LIBS := -lglfw
ifdef DEBUG
	MLX42_FLAGS = -DDEBUG=1
endif
$(MLX42): mlx42-make
	cmake mlx42 -B mlx42/build $(MLX42_FLAGS)
	cmake --build mlx42/build
mlx42-clean:
	rm -rf mlx42/build
clean: mlx42-clean
fclean: mlx42-clean
.PHONY: mlx42-clean mlx42-make

# Project targets
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BINS := $(NAME)
$(NAME): $(OBJS) $(MLX42)
$(NAME): tgt_LDLIBS := -lm $(MLX42LIBS)


# Generic utility targets
.DEFAULT_GOAL := all

.PHONY: all clean .clean fclean .fclean re

all: $(NAME)

clean: .clean
.clean:
	rm -rf $(OBJDIR)

fclean: .fclean
.fclean: .clean
	rm -f $(BINS)

re: fclean all

# Default compiler flags that apply to all targets
def_CFLAGS := -Wall -Wextra -Werror -g
def_CPPFLAGS := -MMD -MP $(addprefix -I ,$(INCDIRS))

# Add sanitizer flags if requested
ifneq (,$(strip $(SANITIZE)))
	def_CFLAGS += -fsanitize=$(SANITIZE)
	def_LDFLAGS += -fsanitize=$(SANITIZE)
endif

# Combine default def_FLAGS, target specific tgt_FLAGS and user-supplied FLAGS
# into one _FLAGS variable to be used in recipes
flagvars = CFLAGS CPPFLAGS LDFLAGS LDLIBS
$(foreach v,$(flagvars),$(eval _$v = $$(strip $$(def_$v) $$(tgt_$v) $$($v))))

# Recipe command to ensure directory for target exists
mktargetdir = @mkdir -p $(@D)

# Default recipes for each type of target
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(mktargetdir)
	$(CC) $(_CPPFLAGS) $(_CFLAGS) -c $< -o $@

$(BINS):
	$(mktargetdir)
	$(CC) $(_LDFLAGS) $^ $(_LDLIBS) -o $@

# Inform make that object files don't need to be remade if the requested
# targets are up to date with respect to the source files.
.SECONDARY: $(OBJS)

# Dependency files to handle #include dependencies
DEPS = $(OBJS:.o=.d)
-include $(DEPS)
