PROJECT(JOSKIPI)

cmake_minimum_required(VERSION 2.6)

set(ver_major "1")
set(ver_minor "1")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)

if(NOT WHOLEPACKAGE)
	find_package(Corpus REQUIRED)
	include_directories(${CORPUS_INCLUDE_DIR})
	#link_directories(${Boost_LIBRARY_DIRS})
else(NOT WHOLEPACKAGE)
	include_directories(${TaggerPackage_SOURCE_DIR})
endif(NOT WHOLEPACKAGE)

find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIR})

if(UNIX)
	add_definitions(-DLINUX)
endif(UNIX)

if(WIN32)
	add_definitions(-DANTLR_IMPORTS)
endif(WIN32)

set(operator_SRC 
	./JOSKIPI/Operators/CondOper.cpp ./JOSKIPI/Operators/OnlyBinCC.cpp
	./JOSKIPI/Operators/LookBinCC.cpp ./JOSKIPI/Operators/AndCC.cpp
	./JOSKIPI/Operators/SimpleOperator.cpp ./JOSKIPI/Operators/HasNumSC.cpp
	./JOSKIPI/Operators/OrCC.cpp ./JOSKIPI/Operators/InSC.cpp
	./JOSKIPI/Operators/AgrPPBinCC.cpp ./JOSKIPI/Operators/Condition.cpp
	./JOSKIPI/Operators/AgrBinCC.cpp ./JOSKIPI/Operators/CatFlt.cpp
	./JOSKIPI/Operators/AgrWeakBinCC.cpp ./JOSKIPI/Operators/AgrFlt.cpp 
	./JOSKIPI/Operators/LookLeftBinCC.cpp ./JOSKIPI/Operators/NotCC.cpp
	./JOSKIPI/Operators/Operator.cpp ./JOSKIPI/Operators/CondElem.cpp
	./JOSKIPI/Operators/IntersecSC.cpp ./JOSKIPI/Operators/TestOper.cpp
	./JOSKIPI/Operators/FixSimpOP.cpp ./JOSKIPI/Operators/AtLeastBinCC.cpp
	./JOSKIPI/Operators/ConstValue.cpp ./JOSKIPI/Operators/DisOper.cpp
	./JOSKIPI/Operators/EqualSC.cpp ./JOSKIPI/Operators/ComplexCond.cpp
	./JOSKIPI/Operators/BinaryComplexC.cpp ./JOSKIPI/Operators/IsBigSC.cpp
	./JOSKIPI/Operators/SimpleCondition.cpp
	./JOSKIPI/Operators/OffsetPosition.cpp
	./JOSKIPI/Operators/LookRightBinCC.cpp)

set(patterns_SRC ./JOSKIPI/Patterns/Pattern.cpp)

set(parser_SRC 
	./JOSKIPI/Parsers/antlr/ANTLRParser.cpp
	./JOSKIPI/Parsers/antlr/ANTLRLexer.cpp ./JOSKIPI/Parsers/SimpleParser.cpp
	./JOSKIPI/Parsers/PatternParser.cpp ./JOSKIPI/Parsers/RulesParser.cpp
	./JOSKIPI/Parsers/ANTLRBasedParser.cpp )

set(rules_SRC ./JOSKIPI/Rules/Rule.cpp ./JOSKIPI/Rules/Action.cpp
	./JOSKIPI/Rules/RuleSet.cpp )

add_library(joskipi SHARED ${operator_SRC} ${parser_SRC} ${rules_SRC} ${patterns_SRC})
add_definitions(-DTEST_WITH_MAIN)

target_link_libraries(joskipi corpus ${Boost_LIBRARIES})

set_target_properties(joskipi PROPERTIES
	VERSION "${ver_major}.${ver_minor}"
	SOVERSION ${ver_major})

if(UNIX)
	install(TARGETS joskipi
		LIBRARY DESTINATION lib)
	
	install(
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/JOSKIPI
		DESTINATION include
		PATTERN "*.svn" EXCLUDE 
		PATTERN "*.cpp" EXCLUDE
		PATTERN "bin" EXCLUDE
		PATTERN "*.bat" EXCLUDE
		PATTERN "*.txt" EXCLUDE
	)
endif(UNIX)
