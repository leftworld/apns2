#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <nghttp2/asio_http2.h>
#include <nghttp2/asio_http2_client.h>
#include <list>
#include <atomic>
using boost::asio::ip::tcp;

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::client;
using namespace std;

class data_fild{
  public:
    string id;
    string url;
    string method;
    string body;
    std::map<string, string> headers;
};

class http2{
  public:
    http2();
    ~http2();
    int init();
    int wait_result();
    int setHost(const char* h);
    int setPort(const char* p);
    int setPem(const char* pem);
    int add2ready(data_fild * data);
    void on_connect(boost::asio::ip::tcp::resolver::iterator it);
    void on_close(uint32_t error_code);
    void on_error(const boost::system::error_code &ec);
    string get_password();
    int set_password(const char * str);
  public:
    boost::system::error_code ec;
    boost::asio::io_service io_service;
    session *sess;
    string host;
    string port;
    string pem_path;
    close_cb close_call;
    connect_cb connect_call;
    error_cb error_call;
    int num;
    std::map<string, string> result;
    std::list<data_fild *> ready;
    string error;
    string password;
    std::map<string, int> response_code;
  private:
    int exec(data_fild *data);
};

