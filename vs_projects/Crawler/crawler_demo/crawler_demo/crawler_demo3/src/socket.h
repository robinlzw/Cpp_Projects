#pragma once
/////////////////////////////////////////////////////////////////////
/// author��suys
/// date:2015/7/30
/////////////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

// @brief �����ͷ�����
enum TypeSocket{BLOCKINGSOCKET, NONBLOCKINGSOCKET};

///////////////////////////////////////////////////////////
/// @brief socket����
/// @note �����ͺͽ�����Ϣ��server��client���й���
///////////////////////////////////////////////////////////
class Socket
{
protected:
	friend class SocketServer;
	friend class ClientSelect;
public:
	int *m_refCounter;
	SOCKET m_socket;     /// @brief socket�����洢
	Socket(SOCKET s);    /// @brief ���캯��������socket
	Socket();            /// @brief ���캯��
private:
	static int socketCounter;  /// @brief socket����
	static void start();       /// @brief winsocket��ʼ�� 
	static void end();         /// @brief winsocket��ʼ������
	
public:
	std::string receiveLine();      /// @brief ����һ����Ϣ
	std::string receiveBytes();     /// @brief ���տ���Ϣ
	void sendLine(std::string str); /// @brief ��������Ϣ
	void sendBytes(const std::string& str);  ///@brief ���Ϳ���Ϣ
	void close();  /// @brief �ر�socket
public:
	Socket(const Socket& sock);        /// @brief �������캯��
	Socket& operator=(Socket& sock);   /// @brief �Ⱥ�����
	virtual ~Socket();                 /// @brief ����������
};


///////////////////////////////////////////////////////////////////
/// @brief �������
///////////////////////////////////////////////////////////////////
class SocketServer :public Socket
{
public:
	/// @brief ����˹��캯��
	/// @note  �������˳���
	/// @param[in] port �󶨶˿�
	/// @param[in] connections ������
	/// @param[in] type �����ͷ�������Ĭ��Ϊ����
	SocketServer(int port, int connections, TypeSocket type = BLOCKINGSOCKET);
	/// @brief �Ƿ���տͻ�������
	Socket* accept_s();
};

//////////////////////////////////////////////////////////////////
/// @brief �ͻ�����
//////////////////////////////////////////////////////////////////
class SocketClient :public Socket
{
public:
	/// @brief �ͻ��˹������
	/// @param[in] host ����
	/// @param[in] port ����˿�
	SocketClient(const std::string &host, int port);
};


//////////////////////////////////////////////////////////////////
/// @brief SocketSelect��
//////////////////////////////////////////////////////////////////
class SocketSelect
{
private:
	fd_set m_fds;
public:
	SocketSelect(Socket const* const s1, Socket const* const s2 = NULL, TypeSocket type = BLOCKINGSOCKET);
	bool readable(Socket const* const s);
};



