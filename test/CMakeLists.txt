enable_testing()

create_test_sourcelist(kinetic_notation_test_files test_driver.c
                       expected_inputs_give_correct_outputs.c)

add_executable(kinetic_notation_tests ${kinetic_notation_test_files})
target_link_libraries(kinetic_notation_tests kinetic_notation)

set(_tests_to_run ${kinetic_notation_test_files})
remove(_tests_to_run test_driver.c)

foreach(_test ${_tests_to_run})
  get_filename_component(_test_name ${_test} NAME_WE)
  add_test(NAME ${_test_name} COMMAND kinetic_notation_tests ${_test_name})
endforeach(_test)
