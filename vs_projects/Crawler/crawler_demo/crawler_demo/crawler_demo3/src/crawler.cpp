#define _CRT_SECURE_NO_WARNINGS
#include "crawler.h"

Crawler::Crawler()
	:m_url("")
{
	// �����ļ��У�����ͼƬ����ҳ�ı��ļ�  
	CreateDirectoryA("./img", 0);
	CreateDirectoryA("./html", 0);
}
Crawler::Crawler(const string& url)
	:m_url(url)
{
	// �����ļ��У�����ͼƬ����ҳ�ı��ļ�  
	CreateDirectoryA("./img", 0);
	CreateDirectoryA("./html", 0);
}

Crawler::~Crawler()
{
}

bool Crawler::parseURL(const string& url, string& host, string& resource)
{
	if (strlen(url.c_str()) > 2000)
	{
		std::cout << "url to long!" << endl;
		return false;
	}
	const char* pos = strstr(url.c_str(), "http://");
	if (pos == NULL)
		pos = url.c_str();
	else
		pos += strlen("http://");
	if (strstr(pos, "/") == 0)
	{
		std::cout << "parseURL error" << endl;
		return false;
	}

	char pHost[100];
	char pResource[2000];
	sscanf(pos, "%[^/]%s", pHost, pResource);
	host = pHost;
	resource = pResource;
	return true;
}

bool Crawler::getHttpResponse(const string&url, string *&response)
{
	string host, resource;
	if (!parseURL(url, host, resource))
	{
		cout << "Can not parse the url!" << endl;
		return false;
	}
	string ret;
	/// ����socket���������ӣ��������ݣ�����������
	try
	{
		SocketClient s(host, 80);
		string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
		s.sendLine(request);
		while (1)
		{
			string block = s.receiveBytes();
			if (block.empty())
				break;
			ret += block;
		}
	}
	catch (const char* s)
	{
		cerr << s << endl;
	}
	catch (std::string s)
	{
		cerr << s << endl;
	}
	catch (...)
	{
		cerr << "unhandled exception\n";
	}
	string::size_type pos = ret.find_last_of("</html>");
	ret = ret.substr(0, pos + 1);
	string *str = new string(ret);
	response = str;
	return true;
}


void Crawler::htmlParse(string& htmlResponse, vector<string>& imgurls)
{
	// �ҵ��������ӣ����뵽queue��
	const char *p = htmlResponse.c_str();
	char *tag = "href=\"";
	const char* pos = strstr(p, tag);
	ofstream ofile("url.txt", ios::app);
	while (pos)
	{
		pos += strlen(tag);
		const char* nextQ = strstr(pos, "\"");
		if (nextQ)
		{
			char *url = new char[nextQ - pos + 1];
			sscanf(pos, "%[^\"]", url);
			string surl = url;  // ת��Ϊstring���ͣ������Զ��ͷ��ڴ�
			//if (surl.find_last_of(".css") != string::npos)
				//continue;
			if (m_visitedUrl.find(surl) == m_visitedUrl.end())
			{
				m_visitedUrl.insert(surl);
				ofile << surl << endl;
				m_hrefUrl.push(surl);
			}
			pos = strstr(pos, tag);
			delete[] url;  // �ͷ�������ڴ�
		}
	}
	ofile << endl << endl;
	ofile.close();

	tag = "<img ";
	const char* attr1 = "src=\"";
	const char* attr2 = "lazy-src=\"";
	pos = strstr(p, tag);
	while (pos)
	{
		pos += strlen(tag);
		const char* pos2 = strstr(pos, attr2);
		if (!pos2 || pos2 > strstr(pos, ">"))
		{
			const char* pos1 = strstr(pos, attr1);
			if (!pos1 || pos1 > strstr(pos, ">"))
			{
				pos = strstr(pos, ">");
				continue;
			}
			else
			{
				pos = pos1 + strlen(attr1);
			}
		}
		else
		{
			pos = pos2 + strlen(attr2);
		}

		const char* nextQ = strstr(pos, "\"");
		if (nextQ)
		{
			char* url = new char[nextQ - pos + 1];
			sscanf(pos, "%[^\"]", url);
			//cout << url << endl;
			string imgUrl = url;
			if (m_visitedImg.find(imgUrl) == m_visitedImg.end())
			{
				m_visitedImg.insert(imgUrl);
				imgurls.push_back(imgUrl);
			}
			pos = strstr(pos, tag);
			delete[] url;
		}
	}
	cout << "end of parse this theml" << endl;
}


string Crawler::toFileName(const string& url)
{
	string fileName;
	fileName.resize(url.size());
	int k = 0;
	for (int i = 0; i < (int)url.size(); ++i)
	{
		char ch = url[i];
		if (ch != '\\'&&ch != '/'&&ch != ':'&&ch != '*'&&ch != '?'&&ch != '"'&&ch != '<'&&ch != '>'&&ch != '|')
			fileName[k++] = ch;
	}
	return fileName.substr(0, k);
}

void Crawler::downLoadImg(vector<string>& imgurls, const string& url)
{
	// ���ɱ����url��ͼƬ���ļ���
	string foldname = toFileName(url);
	foldname = "./img/" + foldname;
	if (!CreateDirectoryA(foldname.c_str(), NULL))
	{
		cout << "Can not create directory:" << foldname << endl;
	}

	for (int i = 0; i < (int)imgurls.size(); ++i)
	{
		/// �ж��Ƿ�ΪͼƬ��bmp��jpg��jpeg��gif
		string str = imgurls[i];
		int pos = str.find_last_of(".");
		if (pos == string::npos)
			continue;
		else
		{
			string ext = str.substr(pos + 1, str.size() - pos - 1);
			if (ext != "bmp" && ext != "jpg" && ext != "jpeg" && ext != "gif" && ext != "png")
				continue;
		}
		//�������е�����
		string *image;
		if (getHttpResponse(imgurls[i], image))
		{
			if (strlen((*image).c_str()) == 0)
				continue;

			const char *p = (*image).c_str();
			const char *pos = strstr(p, "\r\n\r\n") + strlen("\r\n\r\n");
			int temp = pos - p;
			int index = imgurls[i].find_last_of("/");
			if (index != string::npos)
			{
				string imgname = imgurls[i].substr(index, imgurls[i].size());
				ofstream ofile(foldname + imgname, ios::binary);
				if (!ofile.is_open())
					continue;
				ofile.write(pos, (*image).size() - (pos - p));
				ofile.close();
			}
			delete image;
			image = NULL;
		}
	}
}

void Crawler::bfs(const string& url)
{
	string* response;
	/// ��ȡ��ҳ�����ݣ�����response��
	if (!getHttpResponse(url, response))
	{
		cout << "The url is wrong! ignore." << endl;
		return;
	}
	string httpResponse = (*response);
	delete response;
	string filename = toFileName(url);
	ofstream ofile("./html/" + filename + ".html");
	if (ofile.is_open())
	{
		/// �������ҳ���ı�����
		ofile << httpResponse << endl;
		ofile.close();
	}
	vector<string> imgurls;
	/// ��������ҳ������ͼƬ���ӣ�����imgurls����
	htmlParse(httpResponse, imgurls);

	/// ��������ͼƬ��Դ
	downLoadImg(imgurls, url);
}

void Crawler::start()
{
	/// ��ȡ��ҳ�еĳ����ӷ���hrefUrl�У���ȡͼƬ���ӣ�����ͼƬ
	bfs(m_url);

	/// ���ʹ�����ַ��������
	m_visitedUrl.insert(m_url);

	while (m_hrefUrl.size() != 0)
	{
		string url = m_hrefUrl.front();   /// �Ӷ��е��ʼȡ��һ����ַ
		cout << url << endl;
		bfs(url);
		m_hrefUrl.pop();  /// ������֮��ɾ�������ַ
	}
}