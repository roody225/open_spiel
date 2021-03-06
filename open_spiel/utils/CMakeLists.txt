add_library (utils OBJECT
  circular_buffer.h
  data_logger.h
  data_logger.cc
  file.h
  file.cc
  file_logger.h
  json.h
  json.cc
  lru_cache.h
  stats.h
  tensor_view.h
  thread.h
  thread.cc
  threaded_queue.h
)
target_include_directories (utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(circular_buffer_test circular_buffer_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(circular_buffer_test circular_buffer_test)

add_executable(data_logger_test data_logger_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(data_logger_test data_logger_test)

add_executable(file_test file_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(file_test file_test)

add_executable(file_logger_test file_logger_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(file_logger_test file_logger_test)

add_executable(json_test json_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(json_test json_test)

add_executable(lru_cache_test lru_cache_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(lru_cache_test lru_cache_test)

add_executable(stats_test stats_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(stats_test stats_test)

add_executable(tensor_view_test tensor_view_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(tensor_view_test tensor_view_test)

add_executable(thread_test thread_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(thread_test thread_test)

add_executable(threaded_queue_test threaded_queue_test.cc ${OPEN_SPIEL_OBJECTS}
               $<TARGET_OBJECTS:tests>)
add_test(threaded_queue_test threaded_queue_test)
