# heartwall-hip/CMakeLists.txt

add_hecbench_benchmark(
    NAME heartwall
    MODEL hip
    SOURCES main.cu kernel/kernel.cu util/avi/avilib.c util/avi/avimod.c util/file/file.c
    CATEGORIES algorithms
    INCLUDE_DIRS util/avi util/file
)
