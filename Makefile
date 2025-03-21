# Compiler and flags
CC = gcc
# CFLAGS = -std=c17 -Wall -Wextra -Werror -g -pedantic -Iinclude -O0
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -Iinclude -O2
# -Wstrict-prototypes -Wmissing-prototypes
# Source and output
SRC = src/main.c src/util.c src/parser.c src/asm_parser.c src/lexer.c src/stdlib/*.c src/vm/*.c src/vm/instructions/*.c src/vm/instructions/math/*.c
OUTPUT = build/meatsc

# Build target
all: $(OUTPUT)

# Ensure the build directory exists
build:
	mkdir -p build

# Linking and compilation
$(OUTPUT): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Clean target
clean:
	rm build/*

TESTS_BASE_DIR = tests

test: $(OUTPUT)
	@if [ -z "$(dir)" ]; then \
		echo "Running all tests in $(TESTS_BASE_DIR)..."; \
		FAILED=0; SUCCESS=0; \
		for test_file in $$(find "$(TESTS_BASE_DIR)" -type f -name "*.meats"); do \
			echo "Testing: $$test_file"; \
			if ./$(OUTPUT) "$$test_file"; then \
				echo "\033[32mSUCCESS: $$test_file\033[0m"; \
				SUCCESS=$$((SUCCESS + 1)); \
			else \
				echo "\033[31mFAILED: $$test_file\033[0m"; \
				FAILED=$$((FAILED + 1)); \
			fi; \
		done; \
		echo ""; \
		echo "\033[32mSuccessful tests: $$SUCCESS\033[0m"; \
		echo "\033[31mFailed tests: $$FAILED\033[0m"; \
		if [ $$FAILED -ne 0 ]; then \
			echo "\033[31mSome tests failed!\033[0m"; \
			exit 1; \
		else \
			echo "\033[32mAll tests completed successfully!\033[0m"; \
		fi \
	else \
		echo "Running tests in $(TESTS_BASE_DIR)/$(dir)..."; \
		if [ -d "$(TESTS_BASE_DIR)/$(dir)" ]; then \
			FAILED=0; SUCCESS=0; \
			for test_file in $$(find "$(TESTS_BASE_DIR)/$(dir)" -type f -name "*.meats"); do \
				echo "Testing: $$test_file"; \
				if ./$(OUTPUT) "$$test_file"; then \
					echo "\033[32mSUCCESS: $$test_file\033[0m"; \
					SUCCESS=$$((SUCCESS + 1)); \
				else \
					echo "\033[31mFAILED: $$test_file\033[0m"; \
					FAILED=$$((FAILED + 1)); \
				fi; \
			done; \
			echo ""; \
			echo "\033[32mSuccessful tests: $$SUCCESS\033[0m"; \
			echo "\033[31mFailed tests: $$FAILED\033[0m"; \
			if [ $$FAILED -ne 0 ]; then \
				echo "\033[31mSome tests in $(TESTS_BASE_DIR)/$(dir) failed!\033[0m"; \
				exit 1; \
			else \
				echo "\033[32mTests in $(TESTS_BASE_DIR)/$(dir) completed successfully!\033[0m"; \
			fi \
		else \
			echo "\033[31mError: Test directory '$(TESTS_BASE_DIR)/$(dir)' does not exist!\033[0m"; \
			exit 1; \
		fi \
	fi

