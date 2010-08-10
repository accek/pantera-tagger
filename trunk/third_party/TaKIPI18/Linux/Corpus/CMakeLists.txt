PROJECT(Corpus)

cmake_minimum_required(VERSION 2.6)

set(ver_major "1")
set(ver_minor "8")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)

find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

find_package(ICU REQUIRED)
include_directories(${ICU_INCLUDE_DIR})
link_directories(${ICU_LIBRARY_DIRS})

find_package(Morfeusz REQUIRED)

if(UNIX)
	add_definitions(-DLINUX)
endif(UNIX)

set(sourcedatadir
	./Data)

set(destdatadir
	lib/corpusdata/ # must end with a slash (for Corpus::CConfig routines)
)

set(main_SRC
	./main.cpp ./guesser_api.cpp)

set(maindir_SRC
	./Corpus/CharASCII.cpp ./Corpus/Char.cpp
	./Corpus/CharUTF.cpp ./Corpus/Config.cpp
	./Corpus/Corpus.cpp ./Corpus/functions.cpp
	./Corpus/Lexem.cpp ./Corpus/PlainWriter.cpp
	./Corpus/Position.cpp ./Corpus/ResourceManager.cpp
	./Corpus/Sentence.cpp ./Corpus/Sentencer.cpp
	./Corpus/Tag.cpp ./Corpus/Token.cpp
	./Corpus/Transducer.cpp ./Corpus/TransducerNode.cpp
	./Corpus/Value.cpp ./Corpus/WordsDictionary.cpp
	./Corpus/XmlWriter.cpp ./Corpus/entities.cpp)

set(context_SRC
	./Corpus/Context/Context.cpp  ./Corpus/Context/DocumentContext.cpp
	./Corpus/Context/SentenceContext.cpp  ./Corpus/Context/WindowContext.cpp)

set(documents_SRC
	./Corpus/Documents/Document.cpp        ./Corpus/Documents/TaggedDocument.cpp
	./Corpus/Documents/ParsedDocument.cpp  ./Corpus/Documents/TextDocument.cpp)

set(extsentr_SRC
	./Corpus/ExtSentencer/Core.cpp ./Corpus/ExtSentencer/CorpusSentenceReader.cpp
	./Corpus/ExtSentencer/LowLevelSentencer.cpp ./Corpus/ExtSentencer/Sentencer.cpp
	./Corpus/ExtSentencer/SentenceReader.cpp ./Corpus/ExtSentencer/TokenBuffer.cpp)

set(guesser_SRC
	./Corpus/GuessingSystem/EndingTagSet/EndingTagSet.cpp
	./Corpus/GuessingSystem/FormsIdentifier.cpp
	./Corpus/GuessingSystem/EndingTree/EndingTreeNode.cpp
	./Corpus/GuessingSystem/EndingTree/EndingTree.cpp
	./Corpus/GuessingSystem/Guesser.cpp
	./Corpus/GuessingSystem/BaseEnding/BaseEnding.cpp
	./Corpus/GuessingSystem/BaseEnding/BaseEndingDict.cpp
	./Corpus/GuessingSystem/BaseEnding/Prefixes.cpp
	./Corpus/GuessingSystem/GuesserTest/TestingConsoleUI.cpp
	./Corpus/GuessingSystem/GuesserTest/TreeHistogram.cpp
	./Corpus/GuessingSystem/GuesserTest/GuesserTest.cpp
	./Corpus/GuessingSystem/FrequencyList/FrequencyList.cpp)

set(morfeusz_SRC
	./Corpus/Morfeusz/Morfeusz.cpp)

set(parser_SRC
	./Corpus/Parser/Parser.cpp)

set(oldreaders_SRC
	./Corpus/Readers/XmlReader.cpp
	./Corpus/Readers/PlainTextReader.cpp
	./Corpus/Readers/CorporaReader.cpp
	./Corpus/Readers/Reader.cpp)

set(textutils_SRC
	./Corpus/TextUtils/Misc/Misc.cpp ./Corpus/TextUtils/Misc/Timer.cpp
	./Corpus/TextUtils/Corpus/Dump.cpp ./Corpus/TextUtils/Corpus/Corpus.cpp)

set(tokenisation_SRC
	./Corpus/Tokenisation/Core.cpp ./Corpus/Tokenisation/TokenReader.cpp
	./Corpus/Tokenisation/Pretoken.cpp ./Corpus/Tokenisation/Tokeniser.cpp
	./Corpus/Tokenisation/CorpusTokenReader.cpp ./Corpus/Tokenisation/PremorphReader.cpp
	./Corpus/Tokenisation/XmlUtils/FragmentReader.cpp
	./Corpus/Tokenisation/TokLayer.cpp ./Corpus/Tokenisation/Layers/PretokenQueue.cpp
	./Corpus/Tokenisation/Layers/PreSplitLayer.cpp ./Corpus/Tokenisation/Layers/ClassifyLayer.cpp
	./Corpus/Tokenisation/Layers/AbbrevLayer.cpp ./Corpus/Tokenisation/Layers/InputLayer.cpp
	./Corpus/Tokenisation/Layers/CombineLayer.cpp ./Corpus/Tokenisation/Layers/SplitLayer.cpp)


add_library(corpus SHARED ${main_SRC} ${maindir_SRC} ${context_SRC} ${documents_SRC} ${extsentr_SRC}
	${guesser_SRC} ${morfeusz_SRC} ${parser_SRC} ${oldreaders_SRC} ${textutils_SRC} ${tokenisation_SRC})

# Changed by accek: use Data/ folder from source, so that we do not need to install the software.
add_definitions(-DTEST_WITH_MAIN -DGUESSER_DATA_PATH="${CMAKE_INSTALL_PREFIX}/${destdatadir}:${CMAKE_CURRENT_SOURCE_DIR}/Data/")

add_executable(guessertest ./main.cpp)

target_link_libraries(guessertest corpus morfeusz ${Boost_LIBRARIES} icuuc)
target_link_libraries(corpus morfeusz ${Boost_LIBRARIES} icuuc)

set_target_properties(corpus PROPERTIES
	VERSION "${ver_major}.${ver_minor}"
	SOVERSION ${ver_major})

if(UNIX)

	install(TARGETS corpus
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
		DIRECTORY ${sourcedatadir}/
		DESTINATION ${destdatadir}
	)
endif(UNIX)
