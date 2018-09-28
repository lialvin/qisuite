#ifndef _TCP_SPLIT_PKT_H_
#define _TCP_SPLIT_PKT_H_

#include <string>

#define   RECV_SPLI_BUF_SIZE    64*1024
#define   RECV_HALF_SIZE    32*1024   //RECV_BUF_SIZE   half length

typedef  BYTE  unsigned char ;

#pragma pack(1)



#pragma pack()

class    CTcpSplitPkt
{

private:

	//BYTE  *                   m_pDecodeBuf;
	int                       m_nDecodeSize;   
	int                       m_nDecodeIdx;
	bool                      m_bTcpServer;
	int                       m_nLeftLen;
public:

  typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
                                const std::string& message,
                                muduo::Timestamp)> StringMessageCallback;

    
	CTcpSplitPkt( const StringMessageCallback& cb );
   
	int    DealConnectData( BYTE * pData, int nDataSize  );
		
	int                 m_iIdx;
protected:
 
	unsigned char   * m_pTcpBuf;
    void   SplitPkt(BYTE *&  pBuf  , int & nDataSize );
	bool   IsFullPkg(BYTE *& pReadPos,int & nReadLen);
	void   Decode(BYTE * pBuf ,int nPktLen );
	
};


#endif  //_TCP_SPLIT_PKT_H_
