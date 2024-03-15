###	COLORS ####
GREEN:= \033[1;32m
MAGENTA:= \033[1;35m
RED:= \033[1;31m
YELLOW:= \033[1;33m
RESET:= \033[0m

CXX:= c++
DEPFLAGS:= -MMD -MP
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98 $(DEPFLAGS)
DEBUGFLAGS:= -ggdb3 -D__DEBUG__

ifeq ($(shell uname), Linux)
	CXXFLAGS += -D__LINUX__
else ifeq ($(shell uname), Darwin)
	CXXFLAGS += -D__MAC__
endif

RM:= rm -rf

NAME:= libjsonparser.a

INCLUDES:= -I./

SRCS:= JsonParser.cpp

OBJS_DIR:= objects
OBJS:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

DEP:= $(OBJS:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(YELLOW)[ LIBRARY ]$(RESET) $(NAME) is ready.\n"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)[ COMPILE ]$(RESET) $<."

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
	@echo "$(MAGENTA)[ DEBUG ]$(RESET) $(NAME) is ready for debugging."

test: CXXFLAGS += $(DEBUGFLAGS)
test: re
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o tests/test_parser tests/tests.cpp $(NAME)
	@echo "$(MAGENTA)[ TEST ]$(RESET) test_parser is ready."
	@for file in tests/*.json; do \
		echo "Testing $$file"; \
		./tests/test_parser $$file; \
		echo; \
	done

clean:
	@$(RM) $(OBJS_DIR) *.d tests/*.d
	@echo "$(RED)[ DELETE ]$(RESET) Removed object files."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) tests/test_parser
	@echo "$(RED)[ DELETE ]$(RESET) Removed $(NAME).\n"

-include $(DEP)

re: fclean all

.PHONY: clean fclean all re
