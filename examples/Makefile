
OUTPUT_PREFIX = $(OUTPUT_DIR)/ex_

INCLUDE =
LIB = 

debug: clean

	$(CC) $(FLAGS) $(FLAGS_DEBUG) $(FLAGS_SHARED) $(LIB) $(LIB_FMLL) $(INCLUDE) $(INCLUDE_FMLL) ex_test.c -o $(OUTPUT_PREFIX)test # TODO

clean:

	rm -Rf $(OUTPUT_DIR)/$(OUTPUT_PREFIX)*

