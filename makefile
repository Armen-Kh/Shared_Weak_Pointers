TARGET = main.out
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS = $(patsubst %.cpp, %.dep, $(SOURCES))
FLAGS = -O2

$(TARGET) : $(OBJECTS)
	g++ $(FLAGS) $^ -o $@

%.o : %.cpp
	g++ -c $(FLAGS) $< -I./ -o $@

.PHONY : run clean profile gcov memcheck
run : 
	./$(TARGET)

clean : 
	@echo "Runnig clean"
	rm -rf $(TARGET) $(OBJECTS) gmon.out Gcov_Results *.gcno *.gcda Report profile_data.txt coverage.info

profile : clean
	$(MAKE) FLAGS="-pg"
	./$(TARGET)
	@echo "Runnig GPROF"
	gprof -b $(TARGET) gmon.out > profile_data.txt
	
gcov : clean
	$(MAKE) FLAGS="--coverage"
	./$(TARGET)
	@echo "Runnig GCOVE"
	mkdir Gcov_Results
	gcov -r main
	mv *.gcov Gcov_Results
	lcov -t "$(TARGET)" -o coverage.info -c -d .
	genhtml -o Report coverage.info
	xdg-open Report/index.html

memcheck : clean
	$(MAKE) FLAGS="-g -O0"
	valgrind --leak-check=full ./$(TARGET)
