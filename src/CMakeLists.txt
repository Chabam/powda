add_subdirectory(graphics)
add_subdirectory(simulation)
add_subdirectory(task)

target_sources(powda PRIVATE
  logger.cpp
  main.cpp
)
