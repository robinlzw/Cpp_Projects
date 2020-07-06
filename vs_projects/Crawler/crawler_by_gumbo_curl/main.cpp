#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>
#include <windows.h>
#include <atlstr.h>
#include <iostream>
#include <list>
#include <thread>

#include "include/gumbo.h"
#define CURL_STATICLIB
#include "include/curl/curl.h"


#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wldap32.lib")
//
//#ifdef _DEBUG
//#pragma comment(lib,"..\\lib\\win32_debug\\libcurld.lib")
//#pragma comment(lib,"..\\lib\\win32_debug\\libcrypto.lib")
//#pragma comment(lib,"..\\lib\\win32_debug\\libssl.lib")
//#pragma comment(lib,"..\\lib\\win32_debug\\gumbo.lib")
//#else
//#pragma comment(lib,"..\\lib\\win32_release\\libcurl.lib")
//#pragma comment(lib,"..\\lib\\win32_release\\libcrypto.lib")
//#pragma comment(lib,"..\\lib\\win32_release\\libssl.lib")
//#pragma comment(lib,"..\\lib\\win32_release\\gumbo.lib")
//#endif

// �ݹ�HTML��ҳ�Ľڵ㣬��ȡ������ͼƬ������
void search_for_links(GumboNode* node, std::list<std::string> &urls)
{
	if (node->type != GUMBO_NODE_ELEMENT)
	{
		return;
	}
	GumboAttribute* href;
	if (node->v.element.tag == GUMBO_TAG_IMG &&
		(href = gumbo_get_attribute(&node->v.element.attributes, "data-original-src")))
	{
		std::cout << href->value << std::endl;
		urls.push_back(href->value);
	}

	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i)
	{
		search_for_links(static_cast<GumboNode*>(children->data[i]), urls);
	}
}

// ��ȡ��ҳ��curl CURLOPT_WRITEFUNCTION �ص�����
static size_t write_html(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string* strHtml = (std::string*)stream;
	for (size_t i = 0; i < nmemb * size; i++)
	{
		strHtml->append(1, ((char*)ptr)[i]);
	}
	return nmemb * size;
}

// ��ȡͼƬ��curl CURLOPT_WRITEFUNCTION �ص�����
size_t write_pic(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE *)stream);
}

// ����curl����
void request(std::string url, void* write_func, void* container)
{
	CURL *curl_handle;
	//struct myprogress prog;

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	//prog.lastruntime = 0;
	//prog.curl = curl_handle;

	/* set URL to get here */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

	/* Switch on full protocol/debug output while testing */
	//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

	/* disable progress meter, set to 0L to enable and 1L to disable debug output */
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_func);

	// ��Ҫ���֤��
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);

	//curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, xferinfo);
	/* pass the struct pointer into the xferinfo function, note that this is
	an alias to CURLOPT_PROGRESSDATA */
	//curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, &prog);

	/* write the page body to this file handle */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, container);

	/* get it! */
	CURLcode errorCode = curl_easy_perform(curl_handle);

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
}

// ��ȡ��ҳ
void get_html(std::string& html, std::string url)
{
	request(url, write_html, &html);
}

// ��ȡͼƬ
void get_pic(std::string path, std::string url)
{
	// ��url���httpsǰ׺�������Լ���ȡ�ľ���url�������
	url = "https:" + url;

	// ��ȡͼƬ���Ʋ����jpg��׺�������Լ���ȡ�ľ���url�������
	std::string picName = url.substr(url.rfind('/') + 1);
	picName += ".jpg";

	// ��װͼƬ��ȫ·��
	std::string picPath;
	if (path.back() == '/' || path.back() == '\\')
		picPath = path + picName;
	else
		picPath = path + "/" + picName;

	// �����ļ�����������
	FILE *pagefile;
	fopen_s(&pagefile, picPath.c_str(), "wb");
	if (pagefile)
	{
		request(url, write_pic, pagefile);
		fclose(pagefile);
	}
}


int main(int argc, const char** argv)
{
	// ��ȡ��ҳ��һƪ��������
	std::string strUrl = "https://www.jianshu.com/p/9d7e83d16bd7";
	std::string strHtml;
	get_html(strHtml, strUrl);

	// ������ҳ
	const char* input = strHtml.data();
	int input_length = strHtml.length();
	GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions, input, input_length);

	// ��ȡͼƬurl
	std::list<std::string> urls;
	search_for_links(output->root, urls);
	gumbo_destroy_output(&kGumboDefaultOptions, output);

	// ���̻߳�ȡͼƬ
	std::thread *ts = new std::thread[urls.size()];
	int index = 0;
	std::list<std::string>::iterator iter = urls.begin();
	for (; iter != urls.end(); iter++)
	{
		ts[index++] = std::thread(get_pic, "resource/pics", *iter);
	}

	for (size_t i = 0; i < urls.size(); i++)
	{
		ts[i].join();
	}

	delete[] ts;

	system("pause");
	return 0;
}

