#include "stdafx.h"
#include "TextParser.h"

int main()
{
	CTextParser parser;
	parser.LoadFile("TestFile.txt");
	int value;
	parser.GetWord("joshua", &value);

	printf("%d", value);

	return 0;
}