#include "comutil.h"
#include "util.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include "md5.h"

//#import <msxml2.dll> raw_interfaces_only

#include "PAI.h"
#include "BTI.h"
#pragma comment(lib, "pai.lib")
#pragma comment(lib, "bti.lib")
#pragma comment(lib, "comsupp.lib")


CPAI::CPAI()
{
    //  add by jampe
    //_bstr_t strIP = "202.159.139.231";
    //ipPAI_Init(&strIP.GetBSTR());
}


int CPAI::Login(char const* name, char const* pwd)
{
    /*
	LONG lret;
	unsigned char chMD5_PWD[18];
	unsigned char lpBuf[8];
	_bstr_t strName;
	_bstr_t strMD5_PWD;
	_bstr_t strServer;
	//wchar_t temp[100];


	strName=name;
	strMD5_PWD="";
	strServer="";

	strcpy((char*)chMD5_PWD, pwd);
	strMD5_PWD=(char*)chMD5_PWD;

	//MD5Compute((unsigned char*)pwd,chMD5_PWD);

	//for (int i=0;i<16;i++)
	//{
	//	//itoa(chMD5_PWD[i],(char*)lpBuf,16);
	//	sprintf((char*)lpBuf,"%.2x",chMD5_PWD[i]);
	//	strMD5_PWD+=(char*)lpBuf;
	//}

	//strMd5_PWD[16]=strMd5_PWD[17]=0;
	//mbstowcs(temp, (char*)strMd5_PWD, 99);
	//BSTR bstrPwd = ::SysAllocString(temp);

	//BSTR bstrSvr = ::SysAllocString(L"");

	lret = ipPAI_Login(0, cCode_OSBP, 7063, &strName.GetBSTR(), &strMD5_PWD.GetBSTR(), &strServer.GetBSTR());

	//::SysFreeString(bstrSvr);
	//::SysFreeString(bstrPwd);
	//::SysFreeString(bstrName);

	return lret;
    */
    return 1;
}
std::string CPAI::GetPassport()
{
    return ipPAI_Passport();
}

//  add by jampe
std::string CPAI::LastError()
{
    return ipPAI_Reply();
}
//  end add


#include <string>
#include <map>

#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h> // sin_*
#endif
//END includes and defines

namespace HTTP {

    /**
    * Use HTTP::GetRequest to send a HTTP/1.1 GET request and receive text or even binary data.\n
    * Initialize with connect(), send the request by calling request() and obtaion
    * the data with getHeader() and getData(). You might want to use getError() if an error occures.
    * For further information look at the following example.
    * \n\n
    * Example:
    * @code
    * std::string data, header;
    * HTTP::GetRequest gr;
    * gr.setUserAgent("HTTPTool/1.1");
    * gr.addHeader("Accept", "image/jpegn");
    * gr.addHeader("header1", "value1");
    *
    * if ( gr.connect( "www.google.de", 80 ) ) {
    *   if ( gr.request( "index.html" ) ) {
    *     while ( gr.process() );
    *     // you may want to update a progress bar with gr.getRawSize();
    *   }
    * }
    *
    * if ( gr.success() ) {
    *   header = gr.getHeader();
    *   data = gr.getData();
    * } else {
    *   cout << gr.getError();
    * }
    * // now do something with data and header using getData() and getHeader()
    * @endcode
    *
    * @note getFooter() is not supported right now, because it is rarely used.
    * @author Dominik Haumann
    * @version 0.4
    * @todo: LOCATION? -> imho, this should be done by the user :-)
    */
    class GetRequest {

    public:
        /**
        * standard constructor
        */
        GetRequest();

        /**
        * destructor
        */
        virtual ~GetRequest();

        /**
        * set the user agent (UA)
        */
        void setUserAgent(const std::string& ua);

        /**
        * get user agent (UA)
        */
        std::string getUserAgent() const;

        /**
        * add a header entry like "Content-Length" etc. This header
        * will be sent to the server with the request.
        * @param key the key
        * @param value the value
        */
        void addHeader(const std::string& key, const std::string& value);

        /**
        * see above
        * @param key the key
        * @param value the value
        */
        void addHeader(const std::string& key, int value);

        /**
        * remove all headers.
        */
        void clearHeaders();

        /**
        * reset headers, user agent and setup the object for a new connection
        */
        void reset();

        /**
        * connect to a server
        * @param host IP or DNS (eg 80.123.23.120 or www.google.de)
        * @param port port (usually port 80)
        * @return true on success
        */
        bool connect(const std::string& host, int port);

        /**
        * initialize HTTP GET request
        * @param page rest of URL (eg. index.html)
        * @return true on success
        */
        bool request(const std::string& page);

        /**
        * continue to get data
        * @return true, if the transfer is not finished, otherwise false
        */
        bool process();

        /**
        * get header from the answer the server returned.
        * @attention Do not mistake it with addHeader which is used to send
        *            the request.
        * @return header of request
        */
        std::string getHeader() const;

        /**
        * get a value out of the header
        * @param key key of header
        * @return value of key (string) or "" if not found
        */
        std::string getHeaderAsString(const std::string& key) const;

        /**
        * get a value out of the header
        * @param key key of header
        * @return value of key (int)
        */
        int getHeaderAsInt(const std::string& key) const;

        /**
        * @return size of header
        */
        unsigned int getHeaderSize() const;

        /**
        * get data, this may be text or binary
        * @return data as std::string
        */
        std::string getData() const;

        /**
        * @return get size of data
        */
        unsigned int getDataSize() const;

        /**
        * get footer
        * @return footer of request
        */
        std::string getFooter() const;

        /**
        * @return size of footer
        */
        unsigned int getFooterSize() const;

        /**
        * get all data without any modifications
        * @return raw data as std::string
        */
        std::string getRaw() const;

        /**
        * during the process get the growing size with this function
        * @return size of raw data
        */
        unsigned int getRawSize() const;

        /**
        * get error message set by setError()
        * @return error message
        */
        std::string getError() const;

        /**
        * get status of the transfer
        * @return true, if transfer was successful
        */
        bool success() const;

    protected:
        /**
        * set error message returned by getError()
        * @param message error message
        */
        void setError(std::string message);

        /**
        * remove headers and footers
        * @return true on success
        */
        bool splitData();

        /**
        * if the transfer was chunked, remove the size infos
        * @return true on success
        */
        bool postProcessData();

        /**
        * get size of one chunk (helper function for postProcessData)
        * @param n position to begin search
        * @param source string to search in
        * @return size, or std::string::npos if invalid
        */
        unsigned int findSize(unsigned int n, const std::string& source) const;

        /**
        * check, whether this is a chunked transfer
        * @return true, if chunked transfer
        */
        bool isChunkedTransfer() const;

        /**
        * If the transfer mode is 'chunked' and stepping through the sizes
        * ends with the '0' we definitely have reached the end of the data
        * transfer and may stop recieving more data.
        * @note this is necessary because otherwise we have a delay of ~5 seconds
        *       in which our app is not responding. So this is actually a HACK to
        *       make it work smoothly :)
        */
        bool isTransferFinished() const;

        /**
        * the exact length is given by "Content-Length: "
        * @return content length
        */
        unsigned int getContentLength() const;

    private:
        std::string m_data;            ///< contains the requested data
        std::string m_header;          ///< contains the header information
        std::string m_footer;          ///< contains the footer information
        std::string m_rawdata;         ///< contains the raw server answer
        std::string m_errorMessage;    ///< contains the error message
        bool        m_success;         ///< flag, if transfer was successful
        std::string m_host;            ///< host as string (www.google.de or IP address)

        std::map <std::string, std::string> m_additionalHeader; ///< additional header infos

        std::string m_useragent;       ///< user agent (eg. "Mozilla/5.0");

        sockaddr_in server;            ///< server struct including host and port
        int sock;                      ///< socket
    };

}

#include <sstream>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <stdlib.h>

#ifndef WIN32
// unix/linux header files
//#include <tcpd.h> // sockaddr_in
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define closesocket(s) close(s)
#endif

// debug
#include <iostream>
//END includes

namespace HTTP {

    //BEGIN GetRequest
    GetRequest::GetRequest()
    {
        m_useragent = "";
    }

    GetRequest::~GetRequest() { }

    void GetRequest::setUserAgent(const std::string& ua)
    {
        m_useragent = ua;
    }

    std::string GetRequest::getUserAgent() const
    {
        return m_useragent;
    }

    void GetRequest::addHeader(const std::string& key, const std::string& value)
    {
        m_additionalHeader[key] = value;
    }

    void GetRequest::addHeader(const std::string& key, int value)
    {
        // int to string
        std::string str;
        std::stringstream ss;
        ss << value;
        ss >> str;

        addHeader(key, str);
    }

    void GetRequest::clearHeaders()
    {
        m_additionalHeader.clear();
    }

    void GetRequest::reset()
    {
        m_useragent = "";
        clearHeaders();
    }

    bool GetRequest::connect(const std::string& host, int port)
    {
        m_success = true;
        m_host = host;
        unsigned long addr;
        struct hostent *host_info;

#ifdef WIN32
        // initialize TCP for Windows ("winsock")
        short wVersionRequested;
        WSADATA wsaData;
        wVersionRequested = MAKEWORD (1, 1);
        if (WSAStartup (wVersionRequested, &wsaData) != 0) {
            setError("HTTP request error: failed to init windows sockets\n");
            m_success = false;
            return false;
        }
#endif

        // create socket
        sock = socket( PF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            setError("HTTP request error: failed to create socket.");
            m_success = false;
            return false;
        }

        // create socked address of the server
        // it is: type, IP-Address and port number
        memset(&server, 0, sizeof (server));
        if ((addr = inet_addr(host.c_str())) != INADDR_NONE) {
            // host is a numeric IP address
            memcpy((char *)&server.sin_addr, &addr, sizeof(addr));
        } else {
            // convert servername to IP address
            host_info = gethostbyname(host.c_str());
            if (host_info == 0L) {
                setError("HTTP request error: unknown server: " + host + "\n");
                m_success = false;
                return false;
            }
            memcpy((char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
        }

        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        return true;
    }

    bool GetRequest::request(const std::string& page)
    {
        m_success = true;
        m_header = "";
        m_data = "";
        m_rawdata = "";
        m_rawdata.reserve( 1048576 ); // reserve 1 MB

        setError("HTTP request: no error occured.");

        char buffer[8192];
        // connect to server
        if ( ::connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
            setError("HTTP request error: cannot connect to server");
            m_success = false;
            return false;
        }

        // create and send HTTP GET request
        std::string cmd = "GET /" + page + " HTTP/1.1\r\n"
            "Host: " + m_host;

        if ( m_useragent.size() > 0 ) {
            cmd += "\r\nUser-Agent: " + m_useragent;
        }

        std::map <std::string, std::string>::iterator it = m_additionalHeader.begin();
        while ( it != m_additionalHeader.end() ) {
            cmd += "\r\n" + it->first + ": " + it->second;
            it++;
        }

        // add empty line
        cmd += "\n\n";

        sprintf(buffer, cmd.c_str());

        send(sock, buffer, strlen(buffer), 0);

        return true;
    }

    bool GetRequest::process( )
    {
        int count;
        char buffer[8192];

        // get server answer
        count = recv(sock, buffer, sizeof(buffer), 0);

        // NOTE: use append, because this way we support binary data!
        m_rawdata.append(buffer, count);

        // check if we have reached the end of the http-stream
        if (count == 0 || isTransferFinished())
        {
            // close connection and socket
            closesocket(sock);

            // split data from response
            m_success = splitData();

            return false;
        }

        return true;
    }

    bool GetRequest::success() const
    {
        return m_success;
    }

    //BEGIN getter
    std::string GetRequest::getHeader() const
    {
        return m_header;
    }

    std::string GetRequest::getHeaderAsString(const std::string& key) const
    {
        if (m_header.size() > 0) {
            unsigned int pos = m_header.find("\r\n" + key + ": ", 0);
            if (pos == std::string::npos) return "";

            pos += 4 + key.size();

            unsigned int end = m_rawdata.find("\r\n", pos);
            if (end == std::string::npos) return "";

            return m_header.substr(pos, end - pos);
        }

        return "";
    }

    int GetRequest::getHeaderAsInt(const std::string& key) const
    {
        std::string str = getHeaderAsString(key);

        if (str.size() == 0) {
            return 0;
        } else {
            int value;
            std::stringstream ss;
            ss << str;
            ss >> value;
            return value;
        }
    }

    std::string GetRequest::getData() const
    {
        return m_data;
    }

    std::string GetRequest::getFooter() const
    {
        return m_footer;
    }

    std::string GetRequest::getRaw() const
    {
        return m_rawdata;
    }

    unsigned int GetRequest::getHeaderSize() const
    {
        return m_header.size();
    }

    unsigned int GetRequest::getDataSize() const
    {
        return m_data.size();
    }

    unsigned int GetRequest::getFooterSize() const
    {
        return m_footer.size();
    }

    unsigned int GetRequest::getRawSize() const
    {
        return m_rawdata.size();
    }

    std::string GetRequest::getError() const
    {
        return m_errorMessage;
    }
    //END getter

    void GetRequest::setError(std::string message)
    {
        m_errorMessage = message;
    }

    bool GetRequest::isChunkedTransfer() const
    {
        return ( m_rawdata.find("\r\nContent-Length: ", 0) == std::string::npos );
    }

    bool GetRequest::isTransferFinished() const
    {
        if ( !isChunkedTransfer() ) return false;

        // now find the first empty line, as this marks is the end of the header \r\n\r\n
        unsigned int pos = m_rawdata.find("\r\n\r\n", 0);
        if (pos == std::string::npos) return false;
        pos += 4;

        unsigned int size;

        // now enter the loop and step through all the chunks, until
        // the last chunk is marked with 0. If we find that, all data
        // was transferred and we return true.
        while ( (size = findSize(pos, m_rawdata)) != std::string::npos ) {
            if ( size == 0 ) return true;

            // move pos over the chunked-info-line
            pos = m_rawdata.find("\r\n", pos);
            if (pos == std::string::npos) return false;
            pos += 2;
            pos += size + 2;
        }

        return false;
    }

    unsigned int GetRequest::getContentLength() const
    {
        unsigned int pos = m_rawdata.find("\r\nContent-Length: ", 0) + 18; // 18 == strlen("\r\nContent-Length: ");
        unsigned int end = m_rawdata.find("\r\n", pos);

        std::string tmp = m_rawdata.substr(pos, end - pos);

        return atoi( tmp.c_str() );
    }

    bool GetRequest::postProcessData()
    {
        // in detail, the chunked BNF
        //     Chunked-Body   = *chunk
        //                       last-chunk
        //                       trailer
        //                       CRLF

        // chunk          = chunk-size [ chunk-extension ] CRLF
        //                  chunk-data CRLF
        // chunk-size     = 1*HEX
        // last-chunk     = 1*("0") [ chunk-extension ] CRLF

        // chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
        // chunk-ext-name = token
        // chunk-ext-val  = token | quoted-string
        // chunk-data     = chunk-size(OCTET)
        // trailer        = *(entity-header CRLF)

        unsigned int size, pos, prev;

        size = findSize(0, m_data);
        if (size == std::string::npos) {
            m_success = false;
            return false;
        }

        // remove the first line
        pos = m_data.find("\r\n", 0);
        if (pos == std::string::npos) {
            m_success = false;
            return false;
        }

        pos += 2;

        m_data = m_data.erase(0, pos);
        prev = pos = size;

        // delete crlf from first chunk
        m_data = m_data.erase(pos, 2);

        while (true) {
            size = findSize(pos, m_data);

            if (size == std::string::npos) {
                m_success = false;
                return false;
            } else if (size == 0) {
                // last chunk
                m_data = m_data.erase(pos, std::string::npos);
                m_success = true;
                return true;
            }

            // remove the line
            pos = m_data.find("\r\n", pos);
            if (pos == std::string::npos) {
                m_success = false;
                return false;
            }

            pos += 2;
            m_data = m_data.erase(prev, pos - prev);
            pos = prev + size;
            prev = pos;

            // delete crlf from this chunk
            m_data = m_data.erase(pos, 2);
        }

        // never happens
        return true;
    }

    unsigned int GetRequest::findSize(unsigned int n, const std::string& source) const
    {
        unsigned int end, crlf, semicolon, size;

        crlf = source.find("\r\n", n);
        if (crlf == std::string::npos)
            return std::string::npos;

        semicolon = source.find(";", n);
        if (semicolon == std::string::npos)
            semicolon = crlf;

        if (semicolon < crlf)
            end = semicolon;
        else
            end = crlf;

        std::stringstream ss;
        ss << source.substr(n, end - n);
        ss.setf(std::ios_base::hex, std::ios_base::basefield); // hex is the format
        ss >> size;

        //    std::cout << "findSize(" << n << ") == " << size << " (" << source.substr(n, end - n) << ")" << std::endl;

        return size;
    }

    bool GetRequest::splitData()
    {
        // info: http://www.jmarshall.com/easy/http/
        //
        // if we know the size, we can copy the amount of data.
        // otherwise we use the chuked transfer.
        //
        // chunked transfer:
        // Part A
        //   HTTP/1.1 200 OK
        //   Date: Fri, 31 Dec 1999 23:59:59 GMT
        //   Content-Type: text/plain
        //   Transfer-Encoding: chunked
        //
        // Part B
        //   1a; ignore-stuff-here
        // part C
        //   abcdefghijklmnopqrstuvwxyz
        //   10
        //   1234567890abcdef
        // Part D
        //   \r\n0\r\n
        //   some-footer: some-value
        //   another-footer: another-value
        //   [blank line here]


        if ( !isChunkedTransfer() ) {

            // get the exact length of data
            unsigned int size = getContentLength();

            unsigned int pos = m_rawdata.find("\r\n\r\n", 0);
            if (pos == std::string::npos) {
                setError("HTTP request error: no header information available");
                m_success = false;
                return false;
            }

            pos += 4;
            // split header
            m_header = m_rawdata.substr(0, pos);
            // remove header from m_data
            m_data = m_rawdata.substr(pos, size);
            m_footer = ""; // :)

        } else {

            // face part A: extract the header
            // find empty line
            unsigned int pos = m_rawdata.find("\r\n\r\n", 0);
            if (pos == std::string::npos) {
                setError("HTTP request error: no valid header found (chunked)");
                m_success = false;
                return false;
            } else {
                m_header = m_rawdata.substr(0, pos + 2);
            }
            pos += 4;

            // face part D: extract footer
            unsigned int end = m_rawdata.rfind("\r\n0", std::string::npos);
            if (end == std::string::npos) {
                setError("HTTP request error: no valid footer found (chunked)");
                m_success = false;
                return false;
            } else {
                // ha! we got footer
                end += 2;
                end = m_rawdata.find("\r\n", end);
                if (end == std::string::npos) {
                    setError("HTTP request error: no valid footer found (chunked*)");
                    m_success = false;
                    return false;
                }
                end += 2;
                m_footer = m_rawdata.substr(end, std::string::npos);
            }

            // copy data part to m_data
            m_data = m_rawdata.substr(pos, end - pos);

            // face part B and C
            // chunked means that we have many chunks with size/body.
            // what we do now, is to remove all sizes, eg. \r\nsize\r\n
            if (!postProcessData()) {
                setError("HTTP request error: the data part seems to be corrupted (chunked)");
                m_success = false;
                return false;
            }
        }

        return true;
    }
    //END GetRequest

}


int PSS_GetCredit(char const* section, char const* name, std::string& strErr)
{
    int rc = 0;
    char buf[200];
    HTTP::GetRequest gr;

    sprintf(buf, "moliyo/checklogin2kop.aspx?memberpk=%s&productfk=%s", name, section);

    // if (gr.connect("61.152.115.74", 80)) {
    //if (gr.connect("192.168.11.72", 8080)) {
	if (gr.connect("61.152.115.184", 8080)) {
        if (gr.request(buf)) {
            while (gr.process()) {}
            rc = atoi(gr.getData().c_str()); // Data 主体就是返回值
        } else {
            rc = 1;
        }
    } else {
        rc = 2;
    }

    switch (rc)
    {
    case 0: strErr = "succeed"; break;

    case 1: strErr = "send login account failed, please retry later"; break;
    case 2: strErr = "can't connect account server, please retry later"; break;

    case -1: strErr = "System is error! please retry later"; break;
    case -2: strErr = "can't connect bill system! please retry later"; break;
    case -3: strErr = "un activation subarea"; break;
    case -4: strErr = "point is't enough , please bug point first!"; break;
    default: strErr = "query failed, please retry later"; break;
		// Modify by lark.li2008
    /*case 0: strErr = "成功"; break;

    case 1: strErr = "发送帐号登录请求失败，请稍后重试"; break;
    case 2: strErr = "无法连接帐号服务器，请稍后重试"; break;

    case -1: strErr = "系统出现故障，请稍后重试"; break;
    case -2: strErr = "连接帐务系统失败，请稍后重试"; break;
    case -3: strErr = "未激活游戏分区"; break;
    case -4: strErr = "点数不足，请先充值"; break;
    default: strErr = "查询过程出现未知错误，请稍后重试"; break;*/
    }

    return rc;
}


