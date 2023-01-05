# 目标文件名称
target = test

# 正则表达式表示当前目录下所有.c文件
c_source = bit_operation.c

# 配置编译所需要的头文件路径

# 配置编译依赖库文件
LDFLAGS += $(MEIGBASE_OS_LIBS)
LDFLAGS += $(LIBMAL_LIBS)
LDFLAGS += $(LIBAPPS_LIBS)

# .PHONE伪目标
.PHONY: all clean

# 要生成的目标文件
all:
	$(CC) $(c_source) $(CFLAGS) $(LDFLAGS) -o $(target)

clean:
	@find ./ -name "*.o"  | xargs rm -rf
	@find ./ -name "*.so" | xargs rm -rf
