CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(OSRequirment)
set (CMAKE_CXX_STANDARD 11)
SET(IncludedFiles include/red_black_tree.h include/stack.h include/misc.h include/red_black_tree.h include/defs.h
        include/headers.h include/process_struct.h include/clk_utilities.h
        include/queue_utilities.h include/highest_priority_first.h
        include/shortest_remaining_time_first.h include/cpu_util.h include/process_data.h include/generic_queue.h include/round_robin.h include/process_queue.h include/process_red_black_tree.h)
SET (ProcessGeneratorSourceFiles src/process_generator.c
        src/generic_queue.c src/process_queue.c src/cpu_util.c
        src/red_black_tree.c src/stack.c src/misc.c src/process_red_black_tree.c)
SET(ProcessSourceFiles src/process.c)
SET(SchedulerSourceFiles src/process_queue.c src/generic_queue.c src/highest_priority_first.c
        src/shortest_remaining_time_first.c src/round_robin.c
        src/round_robin.c src/process_data.c src/scheduler.c
        src/red_black_tree.c src/stack.c src/misc.c src/process_red_black_tree.c)
SET(ClockSourceFiles src/clk.c  src/generic_queue.c)
SET(FilesGeneratorSourceFiles src/files_generator.c)
add_executable(clock ${IncludedFiles} ${ClockSourceFiles})
add_executable(processGenerator ${IncludedFiles} ${ProcessGeneratorSourceFiles})
add_executable(process ${IncludedFiles} ${ProcessSourceFiles})
add_executable(scheduler ${IncludedFiles} ${SchedulerSourceFiles})
add_executable(filesGenerator ${IncludedFiles} ${FilesGeneratorSourceFiles})
include_directories(${CMAKE_ROOT_DIRECTORY} include)