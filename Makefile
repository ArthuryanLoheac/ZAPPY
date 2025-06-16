##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile for zappy project
##

.PHONY: all clean fclean re run coding_style \
	unit_tests tests_run tests_run_coverage

# ============= OBJECT ============= #

OBJ_DIR = obj

OBJ_MAIN_SERVER = $(SRC_MAIN_SERVER:%.c=$(OBJ_DIR)/%.o)
OBJ_MAIN_GUI = $(SRC_MAIN_GUI:%.cpp=$(OBJ_DIR)/%.o)
OBJ_MAIN_AI = $(SRC_MAIN_AI:%.cpp=$(OBJ_DIR)/%.o)

OBJ_CPP_COMMON = $(SRC_CPP_COMMON:%.cpp=$(OBJ_DIR)/%.o)
OBJ_C_COMMON = $(SRC_C_COMMON:%.c=$(OBJ_DIR)/%.o)
OBJ_SRC_SERVER = $(SRC_SERVER:%.c=$(OBJ_DIR)/%.o)
OBJ_SRC_GUI = $(SRC_GUI:%.cpp=$(OBJ_DIR)/%.o)
OBJ_SRC_AI = $(SRC_AI:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJ_DIR)/*.d

# ============= FLAGS ============= #

FLAGS_SERVER = -MMD -MP \
	-I./zappy_server_src/include \
	-I./libc/include \
	-std=gnu17 -Wall -Wextra -Werror \

FLAGS_GUI =	-MMD -MP \
	$(shell find zappy_gui_src -type d -exec echo -I{} \;) \
	-I./libcpp -I./libc -std=c++17 -Wall -Wextra -Werror
LDFLAGS_GUI = -lIrrlicht

FLAGS_AI = -MMD -MP \
	$(shell find zappy_ai_src -type d -exec echo -I{} \;) \
	-I./libcpp -I./libc -std=c++20 -Wall -Wextra -Werror

FLAGS_CPP_COMMON = -MMD -MP \
	$(shell find lib -type d -exec echo -I{} \;) \
	-std=c++20 -Wall -Wextra -Werror

FLAGS_C_COMMON = -MMD -MP \
	-I./libc/include \
	-std=gnu17 -Wall -Wextra -Werror

FLAGS_TEST = -lcriterion --coverage -include cstdint

FLAGS_LINTER =	\
	--repository=. \
	--quiet \
	--output=vs7	\
	--filter=-legal/copyright,-build/c++17,+build/c++20,-runtime/references \
	--recursive \
	--exclude=tests/ \

# ============= NAMES ============= #

ZAPPY_SERVER = zappy_server
ZAPPY_GUI = zappy_gui
ZAPPY_AI = zappy_ai
COMMON_CPP_LIB = libcpp/libcommon.a
COMMON_C_LIB = libc/libcommon.a

# ============= SOURCES ============= #

SRC_MAIN_SERVER	= zappy_server_src/main.c
SRC_MAIN_GUI = zappy_gui_src/main.cpp
SRC_MAIN_AI	= zappy_ai_src/main.cpp

SRC_CPP_COMMON = $(shell find libcpp -type f -name "*.cpp")
SRC_C_COMMON = $(shell find libc -type f -name "*.c")
SRC_SERVER = $(shell find zappy_server_src -type f -name "*.c" ! -name \
	"main.c")
SRC_GUI	= $(shell find zappy_gui_src -type f -name "*.cpp" ! -name "main.cpp")
SRC_AI = $(shell find zappy_ai_src -type f -name "*.cpp" ! -name "main.cpp")
SRC_TESTS = tests/test_1.cpp \

# ============= RULES ============= #

all: $(ZAPPY_SERVER) $(ZAPPY_GUI) $(ZAPPY_AI) plugins_all

$(COMMON_CPP_LIB): $(OBJ_CPP_COMMON)
	@mkdir -p $(dir $@)
	ar rc $(COMMON_CPP_LIB) $(OBJ_CPP_COMMON)
	ranlib $(COMMON_CPP_LIB)

$(COMMON_C_LIB): $(OBJ_C_COMMON)
	@mkdir -p $(dir $@)
	ar rc $(COMMON_C_LIB) $(OBJ_C_COMMON)
	ranlib $(COMMON_C_LIB)

$(ZAPPY_SERVER): $(COMMON_C_LIB) $(OBJ_SRC_SERVER) $(OBJ_MAIN_SERVER)
	gcc -o $(ZAPPY_SERVER) $(OBJ_SRC_SERVER) $(OBJ_MAIN_SERVER) \
	$(FLAGS_SERVER) $(COMMON_C_LIB)

$(ZAPPY_GUI): $(COMMON_CPP_LIB) $(OBJ_SRC_GUI) $(OBJ_MAIN_GUI)
	g++ -o $(ZAPPY_GUI) $(OBJ_SRC_GUI) $(OBJ_MAIN_GUI) \
	$(COMMON_C_LIB) $(COMMON_CPP_LIB) \
	$(LDFLAGS_GUI)

$(ZAPPY_AI): $(COMMON_CPP_LIB) $(OBJ_SRC_AI) $(OBJ_MAIN_AI)
	g++ -o $(ZAPPY_AI) $(OBJ_SRC_AI) $(OBJ_MAIN_AI) \
	$(COMMON_C_LIB) $(COMMON_CPP_LIB) \
	$(FLAGS_AI)

# ============= CLEANS ============= #

clean:
	rm -rf $(OBJ_DIR)
	rm -f *.gcda *.gcno

fclean: clean
	rm -f $(ZAPPY_SERVER) $(ZAPPY_GUI) $(ZAPPY_AI) \
	$(COMMON_C_LIB) $(COMMON_CPP_LIB)
	rm -f unit_tests
	rm -f plugins/*.so

# ============= COMPILATION ============= #

$(OBJ_DIR)/zappy_server_src/%.o: zappy_server_src/%.c
	@mkdir -p $(dir $@)
	gcc -c $(FLAGS_SERVER) $< -o $@

$(OBJ_DIR)/zappy_gui_src/%.o: zappy_gui_src/%.cpp
	@mkdir -p $(dir $@)
	g++ -c $(FLAGS_GUI) $< -o $@

$(OBJ_DIR)/zappy_ai_src/%.o: zappy_ai_src/%.cpp
	@mkdir -p $(dir $@)
	g++ -c $(FLAGS_AI) $< -o $@

$(OBJ_DIR)/libcpp/%.o: libcpp/%.cpp
	@mkdir -p $(dir $@)
	g++ -c $(FLAGS_CPP_COMMON) $< -o $@

$(OBJ_DIR)/libc/%.o: libc/%.c
	@mkdir -p $(dir $@)
	gcc -c $(FLAGS_C_COMMON) $< -o $@

-include $(OBJ_MAIN_SERVER:.o=.d) $(OBJ_MAIN_GUI:.o=.d) $(OBJ_MAIN_AI:.o=.d)
-include $(OBJ_SRC_SERVER:.o=.d) $(OBJ_SRC_GUI:.o=.d) $(OBJ_SRC_AI:.o=.d)
-include $(OBJ_CPP_COMMON:.o=.d) $(OBJ_C_COMMON:.o=.d)

# ============= OTHERS ============= #

re: fclean all

run: all
	./$(ZAPPY_SERVER) -p 4242 -x 10 -y 10 -n team1 team2 -c 10

coding_style: fclean
	coding-style zappy_server_src/ .
	cat coding-style-reports.log
	rm -f coding-style-reports.log

doxygen:
	cd docs && doxygen Doxyfile
	cd docs && npm run start -- --port 3001

# ============= TESTS ============= #

unit_tests:
	g++ -o unit_tests $(SRC_TESTS) $(FLAGS_TEST)

tests_run: unit_tests
	./unit_tests --verbose

tests_run_coverage: tests_run
	gcovr -r . -e tests/

style_check:
	@cpplint $(FLAGS_LINTER) \
		$(shell find . -type f \( -name '*.cpp' -o -name '*.hpp' \))

# ============ PLUGINS ============ #

COMMON_PLUGINS = \

INCLUDE_SO = -I. \
	-I./zappy_gui_src/include \
	-I./zappy_gui_src/dlLoader/ \
	-I./zappy_gui_src/PluginsManagement \

FLAGS_SO =  -std=c++17 -Wall -Wextra -Werror -lIrrlicht \
			$(INCLUDE_SO) \
            -ldl -g

TEST_SRC = $(shell find zappy_gui_plugins_src -type f -name "*.cpp")

plugins_all:
	@mkdir -p plugins
	@for src in $(TEST_SRC); do \
		plugin_name=$$(basename $$src .cpp); \
		g++ -o plugins/$$plugin_name.so -shared -fPIC $(COMMON_PLUGINS) \
			$$src $(FLAGS_SO); \
	done
