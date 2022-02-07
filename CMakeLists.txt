cmake_minimum_required(VERSION 3.21)
project(laba C)

set(CMAKE_C_STANDARD 99)

add_executable(laba main.c structs/app.h structs/studio.h structs/devTeam.h structs/index.h structs.h io.h io/appIo.c io/teamIo.c db.h io/commonIo.c db/initialization.c db/appDb.c db/common/garbage_operations.c db/common/index_operations.c db/common/file_operations.c db/common/table_operations.c algorithms/binary_search.c algorithms.h db/teamDb.c)
