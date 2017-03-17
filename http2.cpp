#include "http2.h"

http2::http2()  
{
  this->num = 0;
  this->close_call = std::bind(&http2::on_close, this, std::placeholders::_1);
  this->error_call = std::bind(&http2::on_error, this, std::placeholders::_1);
  this->connect_call = std::bind(&http2::on_connect, this, std::placeholders::_1); 
}

void http2::on_connect(boost::asio::ip::tcp::resolver::iterator it)
{
  for(auto m : this->ready)
  {
    this->exec(m);
  }
}

void http2::on_close(uint32_t error_code)
{
  if (--this->num == 0)
  {
    this->sess->shutdown();
  }
}

int http2::add2ready(data_fild * data)
{
  this->ready.push_back(data);
  return 0;
}
void http2::on_error(const boost::system::error_code &ec)
{
  this->error = ec.message();
  this->sess->shutdown();
}

http2::~http2()
{
  for(auto m : this->ready)
  {
    delete (m);
  }
  this->ready.clear();
  if (this->sess)
    delete(this->sess);
  this->sess = NULL;
}

int http2::exec(data_fild *data)
{
  if (!data)
  {
    return -1;
  }
  if (!this->sess)
  {
    return -3;
  }
  string event_id = data->id;
  header_map h = {};
  for(auto m : data->headers)
  {
    header_value a;
    a.sensitive = false;
    a.value = m.second;
    h.insert(std::make_pair(m.first, a));
  }
  auto req = this->sess->submit(this->ec, data->method, data->url, data->body , h);
  if (req == NULL)
  {
    return -2;
  }
  auto ptr = this;
  req->on_response([ptr, event_id](const response& res){
      ptr->response_code[event_id] = res.status_code();
      res.on_data([ptr, event_id](const uint8_t *data, std::size_t len) {
          if (len > 0)
          {
            string str;
            str.append((const char*)data, len );
            if (ptr->result.find(event_id) != ptr->result.end())
            {
              ptr->result[event_id].append(str);
            }else{
              ptr->result[event_id] = str;
            }
          }
          });
      });
  req->on_close(this->close_call);
  this->num++;
  return 0;
}

int http2::wait_result()
{
  if (!this->sess)
  {
    return -1;
  }
  if (this->ready.size() == 0)
  {
    return -2;
  }
  this->io_service.run();
  return 0;
}

int http2::setHost(const char* h)
{
  this->host = h;
  return 0;
}

int http2::setPort(const char* p)
{
  this->port = p;
  return 0;
}

int http2::setPem(const char* pem)
{
  this->pem_path = pem;
  return 0;
}

string http2::get_password()
{
  return this->password;
}

int http2::set_password(const char * str)
{
  this->password = str;
  return 0;
}

int http2::init()
{
  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);
  tls.set_default_verify_paths();
  if (this->pem_path.size() > 0)
  {
    tls.set_verify_mode(boost::asio::ssl::verify_peer);
    if (this->password.size() > 0)
    {
      tls.set_password_callback(bind(&http2::get_password, this));
      tls.use_private_key_file(this->pem_path, boost::asio::ssl::context::pem);
    }
    tls.use_certificate_file(this->pem_path, boost::asio::ssl::context::pem);
  }
  configure_tls_context(this->ec, tls);
  if (this->host.size() == 0)
  {
    return -1;
  }
  if (this->port == "")
  {
    this->port = "443";
  }
  this->sess = new session(this->io_service, tls, this->host, this->port);
  if (!this->sess)
  {
    return -2;
  }
  this->sess->on_connect(this->connect_call);
  this->sess->on_error(this->error_call);
  return 0;
}
