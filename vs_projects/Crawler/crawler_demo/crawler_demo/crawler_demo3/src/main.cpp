#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <fstream>
#include "crawler.h"
using namespace std;

int main()
{
	/// ����һ��ҳ��ͼƬ
	Crawler cr1;
	cr1.bfs("http://tieba.baidu.com/p/3927954592");

	/// ��ȱ���������վͼƬ
	//Crawler cr2("http://tieba.baidu.com/p/3927954592");
	//cr2.start();
	
	system("pause");
	return 0;
}