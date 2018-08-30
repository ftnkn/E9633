include ../Makefile.param

TARGET = can

OBJ_DIR := obj
SUFFIX := c cpp
EXCLUDE_DIRS := $(OBJ_DIR)

SRC_DIRS := $(shell find . -maxdepth 1 -type d)
SRC_DIRS := $(patsubst ./%, %, $(SRC_DIRS))
SRC_DIRS := $(filter-out $(EXCLUDE_DIRS), $(SRC_DIRS))
SRC_DIRS += common
#$(warning  $(SRC_DIRS))

SRC := $(foreach dir, ${SRC_DIRS},			\
$(foreach fix, ${SUFFIX}, $(wildcard ${dir}/*.${fix}))	\
)
SRC := $(patsubst ./%, %, $(SRC))
#$(warning  $(SRC))

OBJS := $(foreach fix, ${SUFFIX}, $(SRC:%.${fix}=${OBJ_DIR}/%.o))
OBJS := $(filter-out $(SRC), $(OBJS))
#$(warning  $(OBJS))

CPPFLAGS += $(foreach dir, ${SRC_DIRS}, -I${dir}/)

LDFLAGS += -L../out/                 \
           -luniapi


USR_LIB := $(QL_SDK_PATH)/lib/libql_common_api.a
USR_LIB += $(QL_SDK_PATH)/lib/libql_peripheral.a

default: first ${OBJS} ${TARGET}
	echo "do nothing"

clean:
	rm -rf $(TARGET) obj ../out/$(TARGET)

$(TARGET): $(OBJS)
	$(LINK.cc) $(OBJS) $(LDFLAGS) $(USR_LIB) $(STD_LIB) -o $@
	cp -af $(TARGET) ../out/

first:
	for dir in ${SRC_DIRS}; do mkdir -p ${OBJ_DIR}/$$dir; done

${OBJ_DIR}/%.o: %.cpp
	$(COMPILE.cpp) $(CPPFLAGS) -c $< -o $@

${OBJ_DIR}/%.o: %.c
	$(COMPILE.c) $(CPPFLAGS) -c $< -o $@
