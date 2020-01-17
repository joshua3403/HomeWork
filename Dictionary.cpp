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
	cout << endl <<  "���ܾ �Է��ϼ��� : ";
	cin >> en_Word;
	cin.clear();
	cout << endl << "�ѱ۴ܾ �Է��ϼ��� : ";
	cin >> kr_Word;
	cin.clear();

	dictionary_en.insert(make_pair(en_Word, kr_Word));
	dictionary_kr.insert(make_pair(kr_Word, en_Word));
	cout << endl <<  "�Է� �Ϸ�Ǿ����ϴ�." << endl;
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
				cout << "���Ͻô� �ܾ ������ ����Ǿ� ���� �ʽ��ϴ�. ";
			else
				cout << dictionary_en.find(words[i])->second << " ";
		}
		else
		{
			auto result = dictionary_kr.find(words[i]);
			if (result == dictionary_kr.end())
				cout << "���Ͻô� �ܾ ������ ����Ǿ� ���� �ʽ��ϴ�. ";
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

		cout << "1. ���� ä���, 2. �����ϱ�, 3. ����\n ���Ͻô� �޴��� ��ȣ�� �������� : ";
		cin >> menu;
		string translate_string("");

		switch (menu)
		{
		case 1 :
			PushWord();
			break;
		case 2:
			cout << "���ϴ� ������ �Է��ϼ��� : " << endl;
			cin.ignore(256, '\n');
			getline(cin, translate_string);
			TranslateString(translate_string);
			cin.clear();
			break;
		case 3:
			exit(0);
			break;
		default:
			cout << "�߸��� �޴��� �Է��ϼ̽��ϴ�." << endl;
			break;
		}
		
	}
}