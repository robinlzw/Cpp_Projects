/*
�ҽ����󴸣���һ�ҳ�����ı༭���Ҹ���У��Ͷ������Ӣ�ĸ������ݹ����ǳ����ˣ���Ϊÿ�춼Ҫȥ����������ƴд���󡣵��ǣ��������������ƽ���Ĺ����з����������ҷ���һ������ƴд����Ľݾ���

1. ����ͬ������ĸ����һ��һ����ƴд����ȥ��һ���ľͺ��������� helllo -> hello
2. ����һ������ĸ��AABB�ͣ�����һ��һ����ƴд����ȥ���ڶ��Ե�һ����ĸ�ͺ��������� helloo -> hello
3. ����Ĺ������ȡ������ҡ�ƥ�䣬�������AABBCC����ȻAABB��BBCC���Ǵ���ƴд��Ӧ�����ȿ����޸�AABB�����ΪAABCC

�������Ǹ���ţ���������ѧ���ھ���ͳ�����ƣ��������ԭ��д��һ���Զ�У����������Ч�ʴӴ���ɡ��ò��˶�ã��Ҿͻ����CEO�����϶��³���ӭȢ�׸��������������۷壬���붼�е�С�����أ�
����
����û�뵽���ұ������ˣ�����ʱ�ϰ����˵�� ����������Ҫ����ҵҵ�����ڿҿҡ������ַ֣���Ҫ���У���һ����һ�С�һ���������У�Ҫ�ǲ��У���һ�в���һ�У�һ�в������в��С��� �����������˺����л��㱵ġ���

�����⣺��ʵ�ִ󴸵��Զ�У�Գ���

��������:
��һ�а���һ������N����ʾ���������������ٸ���У����ַ�����

�������N�У�ÿ��Ϊһ����У����ַ�����

�������:
N�У�ÿ�а���һ�����޸�����ַ�����

��������1:
2
helloo
wooooooow

�������1:
hello
woow
*/

#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<cctype>

// �����ַ���
void correctStr(string &inputStr) {
	int head = 0, next = head + 2;

	while (inputStr[head] != '\0' && inputStr[head + 1] != '\0' && inputStr[next] != '\0')
	{
		string printStr;
		if (inputStr[head] == inputStr[next] && inputStr[head+1] == inputStr[next]) {  // ����3����ͬ�ַ�
			printStr.append(inputStr.substr(0, head + 2));  // ������head+1��
			printStr.append(inputStr.substr(next + 1, inputStr.length() - next - 1));  // ��next+1������
			head = 0;
			next = head + 2;
		}
		else if (inputStr[head] == inputStr[head + 1] && inputStr[head] != inputStr[next] && inputStr[next] == inputStr[next + 1]) {  // AABB			
			printStr.append(inputStr.substr(0, next + 1));  // ������next��
			printStr.append(inputStr.substr(next + 2, inputStr.length() - next - 2));  // ��next+2������
			head = 0;
			next = head + 2;
		}
		else {	
			printStr = inputStr;
			head++;
			next = head + 2;
		}
		inputStr = printStr;
	}
	cout << inputStr << endl;
}


int solve() {

	int m;

	while (cin >> m) {
		vector<string> inputStrArray;
		while (m) {
			string inputStr;
			cin >> inputStr;
			inputStrArray.push_back(inputStr);
			m--;
		}

		for (auto &mystring : inputStrArray) {
			correctStr(mystring);
		}		
	}

	return 0;
}