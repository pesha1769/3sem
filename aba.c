#include <stdio.h>
#include <string.h>
void GenerateString (int n, char* s)
{
	int i = 0;
	int l = 0;
	s[0] = 'a';
	for(i = 1; i<=n; i++)
	{
		printf("%s\n", s);
		l = strlen(s);
		s[l] = ('a' + i);
		strncpy (s + l + 1, s, l);
		s[2 * l + 1] = '\0';
	}
}
int main ()
{
	int n;
	scanf("%d", &n);
	char* s = (char*) malloc (sizeof(char) * (1<<n));
	GenerateString (n, s);
	free(s);
	return 0;
}
