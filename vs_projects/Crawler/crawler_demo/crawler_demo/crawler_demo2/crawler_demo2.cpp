#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include<urlmon.h>
#pragma comment(lib, "urlmon.lib")

int main() {
	TCHAR url[] = TEXT("https://img.paulzzh.tech/touhou/random?site=all&size=all");//һ����������Ķ���projectͼƬAPI
	TCHAR filePath[] = _T("�洫ɫͼ.jpg");
	HRESULT res = URLDownloadToFile(NULL, url, filePath, 0, NULL);
	if (res == S_OK) {
		printf("ɫͼOk��������������˳���\n");
		system("pause");
	}
	else if (res == E_OUTOFMEMORY) {
		printf("��ĵ����ڴ治����\n");
		system("pause");
	}
	else if (res == INET_E_DOWNLOAD_FAILURE) {
		printf("ɫͼ������������\n");
		system("pause");
	}
	else {
		printf("Other error: %d\n", res);
		system("pause");
	}
	return 0;
}
