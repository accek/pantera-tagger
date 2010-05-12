PROJECT(TaKIPI)

cmake_minimum_required(VERSION 2.6)

set(ver_major "1")
set(ver_minor "8")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)

if(NOT WHOLEPACKAGE)
	find_package(Corpus REQUIRED)
	find_package(JOSKIPI REQUIRED)
	include_directories(${CORPUS_INCLUDE_DIR} ${JOSKIPI_INCLUDE_DIR})
else(NOT WHOLEPACKAGE)
	include_directories(${TaggerPackage_SOURCE_DIR} ${JOSKIPI_SOURCE_DIR})
endif(NOT WHOLEPACKAGE)

include_directories(${Boost_INCLUDE_DIR})
find_package(Boost REQUIRED COMPONENTS regex)
link_directories(${Boost_LIBRARY_DIRS})

if(UNIX)
	add_definitions(-DLINUX)
endif(UNIX)

set(sourcedatadir
	./Data)

set(destdatadir
	lib/takipi/ # must end with a slash (for TaKIPI::CConfig routines)
)

# source directories
set(main_SRC
	./TaKIPI/legacy.cpp  ./TaKIPI/main.cpp  ./TaKIPI/takipi_headers.cpp)

set(maindir_SRC
	./TaKIPI/TaKIPI/Config.cpp  ./TaKIPI/TaKIPI/Layers.cpp                ./TaKIPI/TaKIPI/Tagger.cpp
	./TaKIPI/TaKIPI/CutOff.cpp  ./TaKIPI/TaKIPI/LexemAscendingSort.cpp    ./TaKIPI/TaKIPI/Test.cpp
	./TaKIPI/TaKIPI/IO.cpp      ./TaKIPI/TaKIPI/PermutationGenerator.cpp  ./TaKIPI/TaKIPI/UDictionary.cpp)

set(classifiers_SRC
	./TaKIPI/TaKIPI/Classifiers/BagClassifier.cpp
	./TaKIPI/TaKIPI/Classifiers/VotingAverageClassifier.cpp
	./TaKIPI/TaKIPI/Classifiers/LMTreeNode.cpp
	./TaKIPI/TaKIPI/Classifiers/DecisionTreeNode.cpp
	./TaKIPI/TaKIPI/Classifiers/VotingWTAClassifier.cpp
	./TaKIPI/TaKIPI/Classifiers/ClassifierRule.cpp
	./TaKIPI/TaKIPI/Classifiers/NegRuleDecisionTree.cpp
	./TaKIPI/TaKIPI/Classifiers/LMTree.cpp
	./TaKIPI/TaKIPI/Classifiers/NegRuleDecisionTreeNode.cpp
	./TaKIPI/TaKIPI/Classifiers/Ripper.cpp
	./TaKIPI/TaKIPI/Classifiers/Part.cpp
	./TaKIPI/TaKIPI/Classifiers/DecisionTree.cpp
	./TaKIPI/TaKIPI/Classifiers/RuleClassifier.cpp
	./TaKIPI/TaKIPI/Classifiers/LogisticModel.cpp)

set(learning_SRC
	./TaKIPI/TaKIPI/Learning/Filter.cpp
	./TaKIPI/TaKIPI/Learning/Teacher.cpp)

# library and executable definitions

add_library(takipi SHARED ${main_SRC} ${maindir_SRC} ${classifiers_SRC} ${learning_SRC})

add_definitions(-DTEACHER_EXE -DTAKIPI_DATA_PATH="${CMAKE_INSTALL_PREFIX}/${destdatadir}")
add_executable(takipi_exec TaKIPI/main.cpp)

#target_link_libraries(takipi_exec takipi joskipi corpus antlr ${Boost_LIBRARIES} morfeusz icuuc)
target_link_libraries(takipi_exec takipi joskipi ${Boost_LIBRARIES} antlr)
target_link_libraries(takipi corpus joskipi ${Boost_LIBRARIES})
	

set_target_properties(takipi PROPERTIES
	VERSION "${ver_major}.${ver_minor}"
	SOVERSION ${ver_major})

set_target_properties(takipi_exec PROPERTIES
	OUTPUT_NAME takipi)

if(UNIX)
	install(TARGETS takipi
		LIBRARY DESTINATION lib)
	
	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		PATTERN ".svn" EXCLUDE
		PATTERN "bin" EXCLUDE
		PATTERN "build" EXCLUDE	
		PATTERN "CMake*" EXCLUDE
	)
	
	install(
		TARGETS takipi_exec
		RUNTIME DESTINATION bin
	)
	
	
	install(
		DIRECTORY ${sourcedatadir}/
		DESTINATION ${destdatadir}
	)
endif(UNIX)
