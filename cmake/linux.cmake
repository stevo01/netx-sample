set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER    gcc)
set(CMAKE_CXX_COMPILER  g++)
set(AS                  as)
set(AR                  ar)
set(OBJCOPY             objcopy)
set(OBJDUMP             objdump)
set(SIZE                size)

set(THREADX_ARCH linux)
set(THREADX_TOOLCHAIN gnu)

set(LINUX_FLAGS "-g -O0 -pthread -m32")

set(CMAKE_C_FLAGS   "${LINUX_FLAGS} -Wall -Werror -DNX_HTTP_NO_FILEX" CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS "${LINUX_FLAGS} -Wall -Werror -DNX_HTTP_NO_FILEX -fno-rtti -fno-exceptions" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_ASM_FLAGS "${LINUX_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
set(CMAKE_EXE_LINKER_FLAGS "${LINUX_FLAGS} ${LD_FLAGS} -Wl,--gc-sections" CACHE INTERNAL "exe link flags")

# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
