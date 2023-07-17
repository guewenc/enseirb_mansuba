WIDTH ?= 5
HEIGHT ?= 4
MANSUBA_FLAGS = -DWIDTH=$(WIDTH) -DHEIGHT=$(HEIGHT)
CFLAGS = -Wall -Werror -Wall -Wextra -std=c99 -g3 $(MANSUBA_FLAGS)

SRC_DIR = src
SRC_MAIN = $(SRC_DIR)/project.c
SRC_FILES = $(filter-out $(SRC_MAIN), $(wildcard $(SRC_DIR)/*.c))

TST_DIR = tst
TST_MAIN = $(TST_DIR)/tst_main.c
TST_FILES = $(filter-out $(TST_MAIN), $(wildcard $(TST_DIR)/*.c))

INCLUDE_SRC_DIR = $(SRC_DIR)/include
INCLUDE_SRC_FILES = $(wildcard $(INCLUDE_SRC_DIR)/*.h)

INCLUDE_TST_DIR = $(TST_DIR)/include
INCLUDE_TST_FILES = $(filter-out $(TST_MAIN), $(wildcard $(INCLUDE_TST_DIR)/*.h))

all: project

-include $(SRC_FILES:%.c=%.d)

%.d: $(SRC_DIR)/%.c
	gcc -MM -MT$(@:%.d=%.o) $< $(CFLAGS) > $@

test: test_project
	./test_project

%.o: %.c
	gcc -c $(CFLAGS) $<

libgame.a: $(SRC_FILES)
	gcc -c $(CFLAGS) -I$(INCLUDE_SRC_DIR) $^
	ar rcs libgame.a *.o
	rm *.o

project: libgame.a $(SRC_MAIN)
	gcc $(CFLAGS) $(SRC_MAIN) -I$(INCLUDE_SRC_DIR) -o $@ -L. -lgame

test_project: $(TST_FILES) $(TST_MAIN) libgame.a
	gcc $(CFLAGS) -I$(INCLUDE_SRC_DIR) $^ -o $@ -L. -lgame -DWIDTH=5 -DHEIGHT=4

clang: $(SRC_FILES) $(TST_FILES) $(INCLUDE_SRC_FILES) $(INCLUDE_TST_FILES) $(SRC_MAIN) $(TST_MAIN)
	clang-format -i $^

doxygen:
	doxygen dgenerate

clean:
	rm -rf *.o *~ *.a project test_project *.dSYM doc