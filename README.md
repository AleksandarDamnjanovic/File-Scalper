Small Files Remover is small and simple c program (50 lines of code) that is going to remove all files from the working directory, smaller than specified limit. 
You need only two parameters -s that stands for smaller and actual size limit in bytes.
Example: clear.bin -s 50000
With clear.bin be the name of the app after you compile it.
This command will delete all files from the curent directory smaller that 50000 bytes.

How to compile it:
	g++ main.cpp -o clear.bin
