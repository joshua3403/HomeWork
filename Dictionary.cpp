#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

map<string, string> dictionary_en;
map<string, string> dictionary_kr;

void PushWord()
{
	string en_Word("");
	string kr_Word("");
	cout << endl <<  "영단어를 입력하세요 : ";
	cin >> en_Word;
	cin.clear();
	cout << endl << "한글단어를 입력하세요 : ";
	cin >> kr_Word;
	cin.clear();

	dictionary_en.insert(make_pair(en_Word, kr_Word));
	dictionary_kr.insert(make_pair(kr_Word, en_Word));
	cout << endl <<  "입력 완료되었습니다." << endl;
	return;
}

void PrintTranslate(vector<string> words)
{
	for (int i = 0; i < words.size(); ++i)
	{
		if (words[i].at(0)>=0 && words[i].at(0) < 127)
		{
			auto result = dictionary_en.find(words[i]);
			if (result == dictionary_en.end())
				cout << "원하시는 단어가 사전에 등재되어 있지 않습니다. ";
			else
				cout << dictionary_en.find(words[i])->second << " ";
		}
		else
		{
			auto result = dictionary_kr.find(words[i]);
			if (result == dictionary_kr.end())
				cout << "원하시는 단어가 사전에 등재되어 있지 않습니다. ";
			else
				cout << dictionary_kr.find(words[i])->second << " ";
		}
	}
	cout << endl << endl;
}

void TranslateString(string input)
{
	int i = 0;
	vector<string> words;
	string temp("");
	while (true)
	{
		if (input[i] == '\0')
		{
			words.push_back(temp);
			temp.clear();
			break;
		}
		if (input[i] != ' ')
		{
			temp += input[i];
			i++;
			continue;
		}
		else if (input[i] == ' ')
		{
			words.push_back(temp);
			i++;
			temp.clear();
			continue;
		}
	}

	PrintTranslate(words);
}

int main()
{

	while (true)
	{
		int menu = 0;
		rewind(stdin);

		cout << "1. 사전 채우기, 2. 번역하기, 3. 종료\n 원하시는 메뉴의 번호를 누르세요 : ";
		cin >> menu;
		string translate_string("");

		switch (menu)
		{
		case 1 :
			PushWord();
			break;
		case 2:
			cout << "원하는 문장을 입력하세요 : " << endl;
			cin.ignore(256, '\n');
			getline(cin, translate_string);
			TranslateString(translate_string);
			cin.clear();
			break;
		case 3:
			exit(0);
			break;
		default:
			cout << "잘못된 메뉴를 입력하셨습니다." << endl;
			break;
		}
		
	}
}