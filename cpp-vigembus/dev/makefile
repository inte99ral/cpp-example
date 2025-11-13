# 경로의 모든 백슬래시를 슬래시로 변경

#### g++ > 외부 라이브러리 소스코드
SRCS := "D:/Program Files/test.cpp" \
		"C:/Program Files/test.cpp"

OBJS := $(SRCS:%.cpp"=build/%.o")

CXXLSRCS := D:/Program Files/test.cpp \
			C:/main.cpp

CXXLOBJS := $(subst ,#,$(CXXLSRCS))
# CXXLOBJS := $(patsubst %.cpp,build/%.o,$(subst  ,#,$(CXXLSRCS)))

# $(subst :,_,$(SRC))

#  $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(subst /,_,$(subst :,_,$(SRC))))))\

define CXXLSRCS_RULE
$(1):
	@ECHO.
	@ECHO "MACRO $$< -o $$@"
endef


# $(foreach S, $(SOURCES), $(eval $(call DEFINE_RULE, $(S))))

# $(foreach i, $(CXXLSRCS), $(call DEFINE_RULE, $(i)))

# $(call CXXLSRCS_RULE, $(I), $(TARGET))



# I := C:\y.cpp
# TARGET := $(subst :,_,$(I))

# define COMPILE_EXTERNAL_MACRO
# $(2):
# 	@ECHO "MACRO $$< -o $$@"
# endef

# $(eval $(call COMPILE_EXTERNAL_MACRO, $(I), $(TARGET)))

.PHONY: default test
default: test

test:
	@ECHO "$(CXXLSRCS)"
	@ECHO "$(CXXLOBJS)"

# test:
# 	$(foreach I, $(SRCS), @ECHO $(I);)
# # 	@ECHO "$(SRCS)"
# # 	@ECHO "$(OBJS)"