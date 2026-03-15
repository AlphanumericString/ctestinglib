# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/08 21:17:07 by bgoulard          #+#    #+#              #
#    Updated: 2026/01/22 13:11:43 by bgoulard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
.PRECIOUS:	$(NAME)
# -- commands
ECHO	=	$(shell which echo)
RM		=	$(shell which rm)
MKDIR	=	$(shell which mkdir)
CP		=	$(shell which cp)
PRINT	=	$(shell which printf)
AR		=	$(shell which ar)
CC		=	$(shell which clang)
CXX		=	$(shell which clang++)
MAKE_SL =	$(shell which make) --no-print-directory

# -- Defaults vars
#  -- Colors
GRAY        = \e[90m
GREEN       = \e[42m
RED         = \e[41m
RESET       = \e[0m
BOLD        = \e[1m
YELLOW      = \e[33m

OK		=	$(GREEN)
KO		=	$(RED)
LOG_MIN	=	$(GRAY)
LOG_MAJ	=	$(YELLOW)

#  -- Dirs
INCLUDES_DIRS	=	./include ./includes ./inc ./
LIB_DIRS		=	./lib ./libs ./lib64 ./
SRC_DIR			=	src
TST_DIR			=	tests
BUILD_DIR		=	build
COVERAGE_DIR	=	coverage
# -- Files
LIBS_LIST		=	$(shell find $(LIB_DIRS) -name "*.a" -type f -or \
							-name "*.so" -type f 2>/dev/null)
MAKE_SELF		=	$(addprefix $(shell pwd)/, $(lastword $(MAKEFILE_LIST)))
LOGFILE			=	makeinfo.log

# -- Flags
CPPFLGS			=\
				-MMD -MP
WFLAGS			=\
				-Wall -Wextra -Werror -Wno-unused-parameter -Wmissing-prototypes
C_STD			=	c99
CXX_STD			=	c++98
INCLUDES		=	$(addprefix -I, $(INCLUDES_DIRS))

CFLAGS			:=	$(WFLAGS) -std=$(C_STD) $(INCLUDES) $(CPP_FLAGS) $(CPPFLGS)
CXXFLAGS		:=	$(WFLAGS) -std=$(CXX_STD) $(INCLUDES) $(CPP_FLAGS) $(CPPFLGS)

COV_FLAGS		:=\
				-fprofile-instr-generate -ftest-coverage -fcoverage-mapping	\
				-fno-inline -fno-inline-functions -O0

LIBS_LS			=	$(addprefix -L, $(LIB_DIRS))
LD_FLAGS		:=	$(LIBS_LS) $(addprefix -l, $(notdir $(basename $(LIBS_LIST))))

DBG_FLAGS		:=	-g3 -O0
DEBUG_LEVEL		?=	0

# -- pure
TARGET_LANG	=	c
ifeq ($(TARGET_LANG),cpp)
	FLAGS	=	$(CXXFLAGS)
	COMP	=	$(CXX)
	CLGD_PREAMBLE := "CompileFlags:\n"	\
	"    - \"$(WFLAGS)\"\n" \
	"    - \"-std=$(CXX_STD)\"\n" \
	"    - \"-xcpp\"\n"
else ifeq ($(TARGET_LANG),c)
	FLAGS	=	$(CFLAGS)
	COMP	=	$(CC)
	CLGD_PREAMBLE := "CompileFlags:\n"	\
	"    - \"$(WFLAGS)\"\n" \
	"    - \"-std=$(C_STD)\"\n" \
	"    - \"-xc\"\n"
else
	$(error "TARGET_LANG must be either 'c' or 'cpp' but was '$(TARGET_LANG)'")
endif

ifeq ($(DEBUG_LEVEL),0)
	FLAGS	:=	$(FLAGS)
	COV_FLAGS		:= $(COV_FLAGS)
else
	FLAGS	:=	$(FLAGS)	$(DBG_FLAGS) -DDEBUG_LEVEL=$(DEBUG_LEVEL)
	COV_FLAGS	:=	$(COV_FLAGS)	$(DBG_FLAGS) -DDEBUG_LEVEL=$(DEBUG_LEVEL)
endif

BASE_NAME		=	simple_tests
NAME			:=	lib$(BASE_NAME).a
TNAME			:=	$(BASE_NAME)_tester

# -- sources
SRCS			=\
				$(shell find $(SRC_DIR) -name "*.c" -type f 2>/dev/null)
TESTS			=\
				$(shell find $(TST_DIR) -name "*.c" -type f 2>/dev/null)
OBJS			=	$(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
TOBJS			=\
				$(addprefix $(BUILD_DIR)/$(TST_DIR)/, $(patsubst %.c, %.o, $(TESTS)))	\
				$(addprefix $(BUILD_DIR)/$(TST_DIR)/, $(patsubst %.c, %.o, $(SRCS)))
REQS			=	$(OBJS:.o=.d)
TREQS			=	$(TOBJS:.o=.d)

# -- Rules
$(BUILD_DIR)/$(TST_DIR)/%.o:%.c
	@$(PRINT) "$(LOG_MIN)Compiling $(notdir $<)...$(RESET)" && \
	$(MKDIR) -p $(dir $@) 									&& \
	$(CC) $(COV_FLAGS) $(FLAGS) -c $< -o $@ 2>> $(LOGFILE)	&& \
	$(PRINT) "$(OK)Success$(RESET)\n"						|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"

$(BUILD_DIR)/%.o:%.c
	@$(PRINT) "$(LOG_MIN)Compiling $(notdir $<)...$(RESET)" && \
	$(MKDIR) -p $(dir $@) 									&& \
	$(CC) $(FLAGS) -c $< -o $@ 2>> $(LOGFILE)				&& \
	$(PRINT) "$(OK)Success$(RESET)\n"						|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
$(BUILD_DIR)/%.o:%.cpp
	@$(PRINT) "$(LOG_MIN)Compiling $(notdir $<)...$(RESET)" && \
	$(MKDIR) -p $(dir $@)									&& \
	$(CXX) $(FLAGS) -c $< -o $@ 2>> $(LOGFILE)				&& \
	$(PRINT) "$(OK)Success$(RESET)\n"						|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
# -- Targets
-include $(REQS) $(TREQS)

all:$(NAME)
#	$(COMP) $(OBJS) $(FLAGS) -o $(NAME) $(LD_FLAGS)		&&
$(NAME): $(OBJS)
	@$(PRINT) "$(LOG_MAJ)Linking $(NAME)...$(RESET)"	&& \
	$(AR) -rcs $(NAME) $(OBJS)	2>> $(LOGFILE)			&& \
	$(PRINT) "$(OK)Success$(RESET)\n"					&& \
	$(RM) -f $(LOGFILE)									|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
tests: $(TNAME)
$(TNAME): $(TOBJS) $(NAME)
	@$(PRINT) "$(LOG_MAJ)Compiling $(TNAME)...$(RESET)"	&& \
	$(COMP) $(TOBJS) $(FLAGS) -o $(TNAME) $(COV_FLAGS)	\
	-L ./ -l $(BASE_NAME)								&& \
	$(PRINT) "$(OK)Success$(RESET)\n"					&& \
	$(RM) -f $(LOGFILE)									|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
coverage: $(TNAME)
	@$(PRINT) "$(LOG_MAJ)Generating coverage ... $(RESET)"	&& \
	$(PRINT) "$(LOG_MIN)profraw ... $(RESET)\n"				&& \
	./$(TNAME)												; \
	$(PRINT) "$(LOG_MIN)profdata ... $(RESET)"				&& \
	llvm-profdata merge -sparse default.profraw -o 			\
	$(TNAME).profdata										&& \
	$(PRINT) "$(LOG_MAJ)coverage to html ... $(RESET)"		&& \
	llvm-cov show -format=html								\
	-instr-profile=$(TNAME).profdata						\
	-ignore-filename-regex=./$(TST_DIR)/*					\
	--show-branches=count									\
	./$(TNAME) -output-dir=$(COVERAGE_DIR)					&& \
	$(PRINT) "$(OK)Success$(RESET)\n"						|| \
	$(PRINT) "$(KO)Failed$(RESET)\n"

dbg_make:
	@$(ECHO) -e "$(LOG_MIN)SRCS:$(RESET)" $(SRCS)
	@$(ECHO) -e "$(LOG_MIN)OBJS:$(RESET)" $(OBJS)
	@$(ECHO) -e "$(LOG_MIN)FLAGS:$(RESET)" $(FLAGS)
	@$(ECHO) -e "$(LOG_MIN)REQS:$(RESET)" $(REQS)
	@$(ECHO) -e "$(LOG_MIN)CFLAGS:$(RESET)" $(CFLAGS)
	@$(ECHO) -e "$(LOG_MIN)CXXFLAGS:$(RESET)" $(CXXFLAGS)
	@$(ECHO) -e "$(LOG_MIN)LD_FLAGS:$(RESET)" $(LD_FLAGS)
	@$(ECHO) -e "$(LOG_MIN)MAKE_SELF:$(RESET)" $(MAKE_SELF)
clean:
	@$(PRINT) "$(LOG_MAJ)Cleaning build files...$(RESET)"	&& \
	$(RM) -rf $(BUILD_DIR) $(LOGFILE) vgcore.* *.profraw	\
	*.profdata	$(COVERAGE_DIR)								&& \
	$(PRINT) "$(OK)Success$(RESET)\n"						|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
fclean: clean
	@$(PRINT) "$(LOG_MAJ)Cleaning compiled files...$(RESET)"	&& \
	$(RM) -f $(NAME) $(TNAME)									&& \
	$(PRINT) "$(OK)Success$(RESET)\n"							|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
uclean: fclean
	@$(PRINT) "$(LOG_MAJ)Cleaning all dev files...$(RESET)"		&& \
	$(RM) -f .clangd $(LOGFILE) ./tags ./compile_commands.json	\
	.clang-format callgrind.* vgcore.*							&& \
	$(PRINT) "$(OK)Success$(RESET)\n"							|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
re: fclean all
re_env: uclean dev_env
tags: $(SRCS)
	@$(PRINT) "$(LOG_MAJ)Creating tags file...$(RESET)"					&& \
	ctags -R --languages=$(TARGET_LANG) --exclude=.git --exclude=build	\
	 $(prepend --exclude=, $(LIB_DIRS) $(INCLUDES_DIRS))				&& \
	$(PRINT) "$(OK)Success$(RESET)\n"									|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
.clangd: $(MAKE_SELF)
	@$(PRINT) "$(LOG_MAJ)Creating .clangd file...$(RESET)"		&& \
	$(ECHO) -n -e $(CLGD_PREAMBLE) > .clangd					&& \
	for dir in $(INCLUDES_DIRS); do								\
		$(ECHO) "     - \"-I"$(shell pwd)"/"$$dir"\"" >> .clangd;	\
	done														&& \
	$(PRINT) "$(OK)Success$(RESET)\n"							|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
compile_commands.json: $(MAKE_SELF)
	@$(PRINT) "$(LOG_MAJ)Creating compile_commands.json file...$(RESET)"	&& \
	$(MAKE_SL) fclean														&& \
	bear -- $(MAKE_SL) -j													&& \
	$(PRINT) "$(OK)Success$(RESET)\n"										|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
dev_env: .clangd compile_commands.json tags $(MAKE_SELF)
	@$(PRINT) "$(LOG_MAJ)Creating development environment...$(RESET)\n"	&& \
	$(MAKE_SL) dbg_make													&& \
	$(PRINT) "$(OK)Success$(RESET)\n"									|| \
	$(PRINT) "$(KO)Fail$(RESET)\n"
.PHONY: all clean fclean re dbg_make coverage
