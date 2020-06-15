all: 	main_root\
	main_splitter_merger \
	main_searcher \


main_root: main_root.c functions.c
	gcc -g main_root.c functions.c -o main_root

main_splitter_merger: main_splitter_merger.c functions.c
	gcc -g 	main_splitter_merger.c functions.c -o main_splitter_merger

main_searcher: main_searcher.c functions.c
	gcc -g main_searcher.c functions.c -o main_searcher

clean:
	rm -f 	\
		main_root\
		main_splitter_merger \
		main_searcher \
		functions
