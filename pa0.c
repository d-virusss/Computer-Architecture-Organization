#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_NR_TOKENS 32	/* Maximum number of tokens in a command */
#define MAX_TOKEN_LEN 64	/* Maximum length of single token */
#define MAX_COMMAND	256		/* Maximum length of command string */

/***********************************************************************
 * parse_command
 *
 * DESCRIPTION
 *	parse @command, and put each command token into @tokens[] and the number of
 *	tokes into @nr_tokens.
 *
 * A command token is defined as a string without any whitespace (i.e., *space*
 * and *tab* in this programming assignment). For exmaple,
 *   command = "  Hello world   Ajou   University!!  "
 *
 * then, nr_tokens = 4 and tokens should be
 *
 *   tokens[0] = "Hello"
 *   tokens[1] = "workd"
 *   tokens[2] = "Ajou"
 *   tokens[3] = "University!!"
 *
 * Another exmaple is;
 *   command = "  add r0   r1 r2 "
 *
 * then, nr_tokens = 4, and tokens is
 *   tokens[0] = "add"
 *   tokens[1] = "r0"
 *   tokens[2] = "r1"
 *   tokens[3] = "r2"
 *
 *
 * RETURN VALUE
 *	Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static int parse_command(char *command, int *nr_tokens, char *tokens[])
{
	/* TODO
	 * Followings are example code. You should delete them and implement
	 * your own code here
	 */
	int notstr = 1; // command의 시작이 문자가 아닐경우를 판별
	int count = 0; // token의 개수
	int count2 = 0; // 2차원 배열에서의 열
	int first = 1; // 첫 token 동적할당 check하기 위한 변수
	for (int i = 0; i < MAX_COMMAND; i++)
	{
		if ((command[i] == ' ' || command[i] == '\t' || command[i] == '\n'))
		{
			if (notstr == 1)
				continue;
			else
			{
				count++;
				tokens[count] = (char *)malloc(sizeof(char) * MAX_TOKEN_LEN);
				count2 = 0;
				notstr = 1;
			}
		}
		else if (command[i] == '\0')
		{
			if (notstr == 1)
				break;
			else
			{
				count++;
				break;
			}
		}
		else
		{
			if (count == 0 && first == 1)
			{
				tokens[0] = (char *)malloc(sizeof(char) * MAX_TOKEN_LEN);
				first = 0;
			}
			notstr = 0;
			tokens[count][count2] = command[i];
			count2++;
		}
	}

	*nr_tokens = count;
	return 0;
}


/***********************************************************************
 * The main function of this program. SHOULD NOT CHANGE THE CODE BELOW
 */
int main(int argc, const char *argv[])
{
	char line[MAX_COMMAND] = { '\0' };
	FILE *input = stdin;

	if (argc == 2) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[1]);
			return -EINVAL;
		}
	}

	while (fgets(line, sizeof(line), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;

		parse_command(line, &nr_tokens, tokens);

		fprintf(stderr, "nr_tokens = %d\n", nr_tokens);
		for (int i = 0; i < nr_tokens; i++) {
			fprintf(stderr, "tokens[%d] = %s\n", i, tokens[i]);
		}
		printf("\n");
	}

	if (input != stdin) fclose(input);

	return 0;
}