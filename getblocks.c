#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blocks.h"

int main () {
	char query[MAXBLOCKNAME], c;
	size_t len;
	Block_Results results = {};
	
	printf("Find blocks matching what?\n" PROMPT);
	
	while (1) {
		fgets(query, MAXBLOCKNAME, stdin);
		len = strlen(query);
		
		if (len <= 1) {
			printf("You didn't enter anything. Try again!\n" PROMPT);
		} else {
			if (query[len - 1] == '\n')
				query[len - 1] = '\0';
			else /* clear line */
				while (c = getc(stdin), c != '\n' && c != EOF);
			
			if (find_blocks(query, results)) {
				show_blocks(results);
				break;
			}
			else
				printf("No blocks found. Try again!\n" PROMPT);
		}
	}
	
	return 0;
}