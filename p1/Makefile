#
# Sample makefile updated to include the commands to run the Clang Static Analyzer
#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537ps
SCAN_BUILD_DIR = scan-build-out

all: main.o options_processing.o process_list.o stat_file_parser.o
	$(CC) -o $(EXE) main.o options_processing.o process_list.o stat_file_parser.o

main.o: main.c options_processing.h process_list.h stat_file_parser.h
	$(CC) $(WARNING_FLAGS) -c main.c

options_procssing.o: options_processing.c options_processing.h
	$(CC) $(WARNING_FLAGS) -c options_processing.c

process_list.o: process_list.c process_list.h
	$(CC) $(WARNING_FLAGS) -c process_list.c

stat_file_parser.o: stat_file_parser.c stat_file_parser.h
	$(CC) $(WARNING_FLAGS) -c stat_file_parser.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 

