# Путь к компилятору
COMPILER=g++

# Флаги компиляции
FLAGS=-Wno-unused-parameter -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_

# Папка с объектами
BIN_DIR=binary

# Папка с исходниками и заголовками
SRC_DIR=source


all: $(BIN_DIR) run.exe


# Завершает сборку
run.exe: $(addprefix $(BIN_DIR)/, main.o queue.o)
	$(COMPILER) $^ -o run.exe


# Предварительная сборка main
$(BIN_DIR)/main.o: $(addprefix $(SRC_DIR)/, main.cpp queue.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@


# Предварительная сборка queue
$(BIN_DIR)/queue.o: $(addprefix $(SRC_DIR)/, queue.cpp queue.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@


# Создание папки для объектников, если она еще не существует
$(BIN_DIR):
	mkdir $@
