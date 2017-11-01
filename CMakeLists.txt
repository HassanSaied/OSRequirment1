CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(OSRequirment)
set (CMAKE_C_STANDARD 11)
SET(IncludedFiles include/red_black_tree.h include/stack.h include/misc.h include/red_black_tree.h include/defs.h
        include/headers.h include/process_struct.h include/clk_utilities.h
        include/queue_utilities.h include/highest_priority_first.h
        include/srtn_algorithm.h include/cpu_util.h include/process_data.h
        include/generic_queue.h include/round_robin.h include/process_queue.h include/hpf_red_black_tree.h
        include/srtn_queue.h include/logger.h include/integer_queue.h include/double_queue.h)


SET (ProcessGeneratorSourceFiles src/process_generator.c src/queue_utilities.c
        src/generic_queue.c src/process_queue.c src/cpu_util.c
        src/clk_utilities.c)

SET(ProcessSourceFiles src/clk_utilities.c src/process.c)

SET(SchedulerSourceFiles src/process_queue.c src/generic_queue.c src/highest_priority_first.c
        src/srtn_algorithm.c src/round_robin.c
        src/round_robin.c src/process_data.c src/scheduler.c
        src/red_black_tree.c src/stack.c src/misc.c src/hpf_red_black_tree.c src/double_queue.c
        src/srtn_queue.c src/queue_utilities.c src/clk_utilities.c src/logger.c src/integer_queue.c)

SET(ClockSourceFiles src/clk.c  src/generic_queue.c)

SET(FilesGeneratorSourceFiles src/files_generator.c)

add_executable(clock ${IncludedFiles} ${ClockSourceFiles})
add_executable(processGenerator ${IncludedFiles} ${ProcessGeneratorSourceFiles})
add_executable(process ${IncludedFiles} ${ProcessSourceFiles})
add_executable(scheduler ${IncludedFiles} ${SchedulerSourceFiles})
add_executable(filesGenerator ${IncludedFiles} ${FilesGeneratorSourceFiles})
include_directories(${CMAKE_ROOT_DIRECTORY} include)
target_link_libraries(scheduler -lm)