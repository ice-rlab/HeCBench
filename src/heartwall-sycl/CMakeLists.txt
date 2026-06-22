# heartwall-sycl/CMakeLists.txt

add_hecbench_benchmark(
    NAME heartwall
    MODEL sycl
    SOURCES main.c kernel/kernel.cpp util/avi/avilib.c util/avi/avimod.c util/file/file.c
    CATEGORIES algorithms
    INCLUDE_DIRS util/avi util/file
)
