## SMART SHARED POINTER

### For educational purposes, an implementation of a Smart Shared pointer wuth a control block was presented.
### A Weak pointer is also implemented to eliminate circular dependency issues with shared pointer.	

### makefile
	command make:
		Run make file for generate objects(*.o) units, and as a final result execution 
		file(main.out), which represent the use of smart pointers.
	command make profile:
		Make compilation and linking with "-pg" flag, which give as an oportunity
		to see proper time analysis for each project unit.
	command make gcov:
		Make compilation and linking with "--coverage" flag, which give as an oportunity
		to see covearge analysis for each source unit. LCOV options are also demonstrated.
	command make memcheck:
		Running with Valgring (Make compilation and linking with "-g" flag and
		make Memcheck run with --leak-check=full option)



