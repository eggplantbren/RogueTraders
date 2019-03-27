CXX = g++
FLAGS = -std=c++17
INCLUDE = -I $(DNEST4_PATH)
LINK = -L $(DNEST4_PATH)/DNest4/code
OPTIM = -O2 -g
WARN = -Wall -Wextra -pedantic
ALL = $(FLAGS) $(INCLUDE) $(OPTIM) $(WARN)

default:
	$(CXX) $(ALL) -c RogueTraders/Options.hpp
	$(CXX) $(ALL) -c RogueTraders/Population.hpp
	$(CXX) $(ALL) -c main.cpp
	$(CXX) $(LINK) -pthread -o main main.o -lpthread -ldnest4 -lyaml-cpp
	rm -f main.o RogueTraders/*.gch


tidy:
	clang-tidy main.cpp -- $(INCLUDE)
