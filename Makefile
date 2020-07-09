PROGRAM_NAME=ased
SRC_DIR=src
O_DIR=obj
CXX=g++
cpp_dirs=
SRC=src

SRC_FILES= $(wildcard $(SRC)/*.cpp $(addsuffix /*.cpp,$(addprefix $(SRC)/,$(cpp_dirs))))
H_FILES=$(SRC_DIR) $(addprefix $(SRC_DIR)/,$(cpp_dirs))
OBJ_FILES=$(patsubst $(SRC)/%.o,$(O_DIR)/%.o, $(SRC_FILES:.cpp=.o))

VPATH=$(SRC_DIR) $(addprefix $(SRC)/,$(cppdirs))

CXX_FLAGS=$(addprefix -I, $(H_FILES)) -O3 -Wall -Wno-multichar -lm -std=c++17 -lutil -pthread -lncurses

all: $(PROGRAM_NAME) | run

compile: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(external_files)

$(O_DIR)/%.o: %.cpp | $(addprefix $(O_DIR)/,$(cpp_dirs))
	$(CXX) -c -o $@ $< $(CXX_FLAGS)


$(addprefix $(O_DIR)/,$(cpp_dirs)):
	mkdir -p $@

.PHONY: clean run
clean:
	rm -rf $(O_DIR)
	rm $(PROGRAM_NAME)

run: $(PROGRAM_NAME)
	./$<
