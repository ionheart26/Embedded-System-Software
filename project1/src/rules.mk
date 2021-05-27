# 파일이름엔 무조건 $(SRC_DIR) 붙이기
# .PHONY는 command를 무조건 실행(dependency는 확인)

include $(SRC_DIR)/EXE.mk

CC = arm-none-linux-gnueabi-gcc
# CC= gcc
CFLAGS = -pthread -static -Wall
BUILD_DIR = $(SRC_DIR)/../build
OBJ_DIR = $(BUILD_DIR)/obj
CFILE = $(wildcard *.c)
OBJECTS = $(addprefix $(OBJ_DIR)/, $(CFILE:.c=.o))


$(SRC_DIR)/../$(EXE) : build.lib build.mode build.process build.main 
	$(CC) $(CFLAGS) -o $@ $(OBJ_DIR)/*.o

.PHONY : build.lib
build.lib :
	make -C $(SRC_DIR)/lib build

.PHONY : build.mode
build.mode :
	make -C $(SRC_DIR)/mode build

.PHONY : build.process
build.process :
	make -C $(SRC_DIR)/process build

.PHONY : build.main
build.main :
	make -C $(SRC_DIR) build



$(OBJECTS) : $(wildcard *.c) $(wildcard *.h) $(OBJ_DIR) 
	$(CC) $(CFLAGS) -o $@ -c $(subst .o,.c,$(notdir $@))
 
$(OBJ_DIR) :
	@mkdir -p $(OBJ_DIR)
	


.PHONY : adb
adb : $(SRC_DIR)/../$(EXE)
	adb push $^ /data/local/tmp

.PHONY : clean
clean :
	rm -rf $(BUILD_DIR) $(SRC_DIR)/../$(EXE)
