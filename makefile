HEADERS := \
	common_defs.h \
	bridge.h \
	exception.h \
	hiker.h \
	solution_strategy.h \
	solution_utils.h \
	strategy_sequential.h \
	world.h \
	world_impl.h \
	world_util.h


#YAML_CPP_INCLUDE := ../yaml-cpp-0.6.0/include
#YAML_CPP_LIB := ../yaml-cpp-0.6.0/lib
YAML_CPP_MODULE := yaml-cpp


COMPILER_OPTIONS = -std=c++11 -g -ggdb -pedantic -Wall -Wextra -Werror

.PHONY: default
default: unit_tests app-yaml-reader

unit_tests: unit_tests.cpp $(HEADERS) makefile
	g++ $(COMPILER_OPTIONS) $< -o $@

app-yaml-reader: main.cpp $(HEADERS) makefile
	g++ $(COMPILER_OPTIONS) -I$(YAML_CPP_INCLUDE) -L$(YAML_CPP_LIB) -l$(YAML_CPP_MODULE) $< -o $@


.PHONY: clean
clean:
	rm -f unit_tests
	rm -f app-yaml-reader
