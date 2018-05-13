cmake_minimum_required(VERSION 3.2)

project(kkmeans VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project(kkmeans VERSION 0.0)

include_directories(
	include
)

add_executable(kkmeans
					main.cpp )

target_link_libraries(kkmeans dlib openblas)

set_target_properties(kkmeans PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install(TARGETS kkmeans RUNTIME DESTINATION .)
install(FILES tests/kkmeans_ex.txt DESTINATION .)
install(FILES tests/stellar.txt DESTINATION .)


set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT __shurik@list.ru)

include(CPack)

