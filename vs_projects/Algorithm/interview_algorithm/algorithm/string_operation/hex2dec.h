/*
д��һ�����򣬽���һ��ʮ�����Ƶ������������ֵ��ʮ���Ʊ�ʾ��������ͬʱ���� ��

�������� :
����һ��ʮ�����Ƶ���ֵ�ַ�����

������� :
�������ֵ��ʮ�����ַ�����

��������1 :
0xA

�������1 :
10
*/

#include<string>
#include<iostream>
using namespace std;
#include<math.h>
#include<cctype>

int solve1() {

	string inputStr;
	
	while (cin >> inputStr) {
		int result = 0;
		if (isxdigit((*(inputStr.c_str())))) {
			sscanf_s(inputStr.c_str(), "%x", &result);
			cout << result << endl;
		}
	}
	
	return 0;
}


int solve2() {

	string inputStr;

	while (cin >> inputStr) {
		long result = 0;
		if (isxdigit((*(inputStr.c_str())))) {
			char *endptr;
			result = strtol(inputStr.c_str(), &endptr, 16);
			cout << result << endl;
		}
	}

	return 0;
}

