all: hello_os.c
	gcc -o os_hello hello_os.c

clean: 
	rm os_hello
