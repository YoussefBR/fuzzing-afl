// program adapted from an example by mykter

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#define INPUTSIZE 100

int process(char *input)
{
	char *out;
	char *rest;
	long len;
    int i;
	if (strncmp(input, "u ", 2) == 0)
	{ // upper case command
		if(strlen(input) < 3) goto invalid_in;
		errno = 0;
		len = strtol(input + 2, &rest, 10); // how many characters of the string to upper-case
		if(errno == ERANGE || rest == input + 2) goto invalid_in; // if over/under flow occurs or no valid conversion is performed

		rest += 1;							// skip the first char (should be a space)
		if(len < 0 || len > strlen(rest)) goto invalid_in;
		out = malloc(len + strlen(input));  // could be shorter, but play it safe
		if (len > strlen(input))
		{
			printf("Specified length %ld was larger than the input!\n", len);
			return 1;
		}
		else if (out == NULL)
		{
			printf("Failed to allocate memory\n");
			return 1;
		}
		for (i = 0; i != len; i++)
		{
			if(rest[i] >= 97 && rest[i] <= 122){
				out[i] = rest[i] - 32; // only handles ASCII
			} else if(rest[i] >= 65 && rest[i] <= 90){
				out[i] = rest[i];
			}else{
				goto invalid_in;
			}
		}
		out[len] = 0;
		strcat(out, rest + len); // append the remaining text
		printf("%s", out);
		free(out);
	}
	else if (strncmp(input, "head ", 5) == 0)
	{ // head command
		if (strlen(input) > 6)
		{
			errno = 0;
			len = strtol(input + 4, &rest, 10);
			if(errno == ERANGE || rest == input + 4) goto invalid_in; // if over/under flow occurs or no valid conversion is 
			rest += 1;		  // skip the first char (should be a space)
			if(len < 0 || len > strlen(rest)) goto invalid_in;
			rest[len] = '\0'; // truncate string at specified offset
			printf("%s\n", rest);
		}
		else
		{
			fprintf(stderr, "head input was too small\n");
		}
	}
	else
	{
		return 1;
	}
invalid_in:
	return 0;
}

int main(int argc, char *argv[])
{
	char *usage = "Usage: %s\n"
				  "Text utility - accepts commands and data on stdin and prints results to stdout.\n"
				  "\tInput             | Output\n"
				  "\t------------------+-----------------------\n"
				  "\tu <N> <string>    | Uppercased version of the first <N> bytes of <string>.\n"
				  "\thead <N> <string> | The first <N> bytes of <string>.\n";
	char input[INPUTSIZE + 1] = {0};

	// Slurp input
	if (read(STDIN_FILENO, input, INPUTSIZE) < 0)
	{
		fprintf(stderr, "Couldn't read stdin.\n");
	}
	
	input[INPUTSIZE + 1] = '\0';

	int ret = process(input);
	if (ret)
	{
		fprintf(stderr, usage, argv[0]);
	};
	return ret;
}