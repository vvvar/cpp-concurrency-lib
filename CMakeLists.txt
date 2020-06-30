project(cpp-concurrency-lib)

add_library(${PROJECT_NAME} STATIC
	src/Concurrency.cpp
  src/EventLoop.cpp
  include/${PROJECT_NAME}/Concurrency.hpp
  include/${PROJECT_NAME}/AbstractTask.hpp
  include/${PROJECT_NAME}/Task.hpp
  include/${PROJECT_NAME}/EventLoop.hpp
	include/${PROJECT_NAME}/CppConcurrencyLib.hpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_11
)
