main : mdbg.o
	@echo "linking $@ dependences $^"
	g++ -o mdbg mdbg.o
.cpp.o:
	@echo "compiling $< => $@"
	g++ -c -o mdbg.o main.cpp -std=c++11

clean:
	rm -rf *.o mdbg
