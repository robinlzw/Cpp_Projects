
// gaoyuanDlg.h : ͷ�ļ�
//

#pragma once
#include <afxinet.h>
#include <string>
#include <map>
using namespace std;

typedef struct data_s
{
	string stock;
	string price;
	string name;
}s_data_s;
// CgaoyuanDlg �Ի���
class CgaoyuanDlg : public CDialogEx
{
// ����
public:
	bool m_isInit;
	int m_nLeft;
	CString m_szText;
	bool bIsShow;
	char m_text[1024]; 
	map<int, s_data_s> mapStock_;
	CHotKeyCtrl	m_HotKey;
	//int count;
	CgaoyuanDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void TuoPan();//��������
	void GetQuotationData(map<int, s_data_s> *pMapStockInfo);
	bool OpenCfgFile();
	NOTIFYICONDATA nid;//NOTIFYICONDATA�ṹ������ϵͳ������������ͼ�����Ϣ
	LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
// �Ի�������
	enum { IDD = IDD_GAOYUAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	void OnBtnHotKey();
	void OnDestroyHotKey();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);  // �ȼ���Ϣ��Ӧ����������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
