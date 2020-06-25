#pragma once
/////////////////////////////////////////////////////////////////////
/// author��suys
/// date:2015/7/30
/////////////////////////////////////////////////////////////////////
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS //�������궨�弴������
#ifndef _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
static_assert(false, "<hash_map> is deprecated and will be REMOVED. "
	"Please use <unordered_map>. You can define "
	"_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS "
	"to acknowledge that you have received this warning.");
#endif /* _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <hash_set>
#include "socket.h"

using namespace std;

//////////////////////////////////////////////////////////
/// @brief ������
//////////////////////////////////////////////////////////
class Crawler
{
private:
	string m_url;                     /// @brief �����ʼURL
	queue<string> m_hrefUrl;          /// @brief ��������δ���ʵ�href
	hash_set<string> m_visitedUrl;    /// @brief ���������Ѿ����ʹ���url
	hash_set<string> m_visitedImg;    /// @brief �����Ѿ����ʹ�������ͼƬ
public:
	/// @brief ����URL���������������Դ��
	/// @param[in]  url Ҫ������URL
	/// @param[out] host ����������host
	/// @param[out]  resource ������������Դ��
	bool parseURL(const string& url, string& host, string& resource);

	/// @brief ʹ��Get���󣬻������ҳ��
	/// @param[in]  url �����url
	/// @param[out] response ����˷��ص�����ҳ������
	bool getHttpResponse(const string&url, string *&response);

	/// @brief ����ȡ�õ�ǰҳ������е�href�Լ�ͼƬURL
	/// @param[in]  htmlResponse htmlҳ�������
	/// @param[out] imgurls �洢����ͼƬ��url��ַ
	void htmlParse(string& htmlResponse, vector<string>& imgurls);

	/// @brief ��urlת��Ϊ�ļ���
	/// @param[in] url ��Ҫת����url
	string toFileName(const string& url);

	/// @brief ��ͼƬ���ص�img�ļ�����
	/// @param[in] imgurls ����ͼƬ��url��ַ
	/// @param[in] url ͼƬ����Ӧ��url��ַ
	void downLoadImg(vector<string>& imgurls, const string& url);
	
	/// @brief ����һ��url�µ�����ͼƬ
	/// @param[in] url ��Ҫ����ͼƬ��url
	void bfs(const string& url);

	/// @brief ��ȱ�����������url��ͼƬ
	void start();
public:
	/// @brief ���캯��
	Crawler();
	Crawler(const string &url);
	~Crawler();
};

