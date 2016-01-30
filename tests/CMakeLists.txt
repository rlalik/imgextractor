set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(cppunit)

file(GLOB UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_tests.cpp" )

link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

add_executable(UnitTester test_runner.cpp ${UnitTests_SRCS} ../functions.cpp)
target_link_libraries(UnitTester ${CPPUNIT_LIBRARIES} ${ROOT_LIBRARIES})

foreach(test ${UnitTests_SRCS})
	get_filename_component(TestName ${test} NAME_WE)
	message(STATUS "Adding test ${TestName}")
	add_test(${TestName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/UnitTester ${TestName})
endforeach(test)

# add_custom_command(TARGET MainTest
# 	POST_BUILD
# 	COMMAND ctest --output-on-failure)

# add_custom_target(MainTest ALL UnitTester)