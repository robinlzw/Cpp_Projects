#pragma once
#include <winsock2.h>
#include<iostream>
#include <fstream>
#include<string>
#include<vector>
#include<queue>
#include<map>

using namespace std;
#pragma comment(lib, "ws2_32.lib")

class urlDownload {
private:
	//socket
	WSADATA ws;
	SOCKET sock;

	//data
	queue<string>htmlurl;
	vector<string>comurl;
	vector<string>imageurl;
	map<string, int>urlmap;
	//addr
	string host;
	string filepath;
	//log
	ofstream wlog;
	//��ȡ��Դ����
	string getFileName(const string &filepath);
	void parse_url(const string &url);
	//��ȡhtml���img url
	void getImageUrl(const string &allHtml);
	//��ȡcom url
	void getComUrl(const string &allHtml);
	//log
	void log(const string &str);
	string iToa(int num);
	//http Get����
	void request(const string &url);
	//����������ҳ�õ���html�ı�
	void recvUrl();
public:
	urlDownload() {
		CreateDirectoryA("./img", 0);
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
	~urlDownload() {
		WSACleanup();
	}
	//������ҳ��ͼƬ
	void downLoadImage(const string &url);
	void run(const string &url);
};
