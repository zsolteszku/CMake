set(CMAKE_CROSSCOMPILING 1)

# Executable: Return error code different from 0
add_executable(generated_exe simple_src.cxx)

# DoesNotUseEmulator
add_custom_target(generate_output1 ALL
  ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/output1)

# DoesNotUseEmulator: The command will fail if emulator is prepended
add_custom_target(generate_output2 ALL
  ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:generated_exe>"
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/output2
  DEPENDS generated_exe)

# UsesEmulator: The command only succeeds if the emulator is prepended
#               to the command.
add_custom_target(generate_output3 ALL
  $<TARGET_FILE:generated_exe>
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/output3
  DEPENDS generated_exe)

# UsesEmulator: The command only succeeds if the emulator is prepended
#               to the command.
add_custom_target(generate_output4 ALL
  generated_exe
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/output4
  DEPENDS generated_exe)
