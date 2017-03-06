#include "http2.h"
int main()
{
  return 0;
}

http2::http2(): tls(boost::asio::ssl::context::sslv23) , is_connect(false)
{
  this->close_call = std::bind(&http2::on_close, this, std::placeholders::_1);
  this->error_call = std::bind(&http2::on_error, this, std::placeholders::_1);
  this->connect_call = std::bind(&http2::on_connect, this, std::placeholders::_1); 
}

void http2::on_connect(boost::asio::ip::tcp::resolver::iterator it)
{
  std::lock_guard<std::mutex> lk(this->mut);  
  this->is_connect = true;
  this->connect_cond.notify_one();  
}

void http2::on_close(uint32_t error_code)
{
  if (--this->num == 0)
  {
    this->sess->shutdown();
  }
}

void http2::on_error(const boost::system::error_code &ec)
{
  std::cerr << "error: " << ec.message() << std::endl;
}

http2::~http2()
{
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
  string id = data->id;
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
  req->on_response(
      [this, &id](const response& res){
      res.on_data([this, &id](const uint8_t *data, std::size_t len) {
          std::cerr.write(reinterpret_cast<const char *>(data), len);
          string str;
          str.append((const char*)data, len);
          this->result[id] = str;
          });
      });
  req->on_close(this->close_call);
  delete (data);
  return 0;
}

int http2::wait_result()
{
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

int http2::init()
{
  this->tls.set_default_verify_paths();
  if (this->pem_path.size() > 0)
  {
    this->tls.use_certificate_chain_file(this->pem_path);
  }
  if (this->host.size() == 0)
  {
    return -1;
  }

  if (this->port == "")
  {
    this->port = "443";
  }
  this->sess = new session(this->io_service, this->tls, this->host, this->port);
  this->sess->on_connect(this->connect_call);
  this->sess->on_error(this->error_call);
  std::unique_lock<std::mutex> lk(this->mut);  
  this->connect_cond.wait(lk,[this]{return this->is_connect;});  
  lk.unlock();  
  return 0;
}
/*
   int main(int argc, char *argv[]) {
   boost::system::error_code ec;
   boost::asio::io_service io_service;
   boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);
   tls.set_default_verify_paths();
// disabled to make development easier...
//   // tls_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
configure_tls_context(ec, tls);
tls.use_certificate_chain_file

tls.use_certificate_file();
bitorost::asio::ssl::context::file_format::pem

//connect to localhost:3000
session sess(io_service, tls, "gcm-http.googleapis.com", "443");

sess.on_connect([&sess](tcp::resolver::iterator endpoint_it) {
boost::system::error_code ec;

auto printer = [](const response &res) {
for(auto m : res.header())
{
std::cerr<<m.first;
std::cerr<<":";
std::cerr<<m.second.value;
std::cerr<<std::endl;
}

res.on_data([](const uint8_t *data, std::size_t len) {
std::cerr.write(reinterpret_cast<const char *>(data), len);
//std::cerr << std::endl;
});
};

std::size_t num = 5000;
auto count = std::make_shared<int>(num);

std::string data = "{\"data\":{\"message\":{\"id\":0,\"title\":\"Hello ";
std::string data1 = "\",\"text\":\"\\u2728 TheVoice miss your voice! Come & sing new songs we just released!\",\"time\":1488513501,\"action_url\":\"https:\\/\\/m.starmakerstudios.com\\/stmaker_activity\\/?promotion_id=26&showBar=0\",\"kind\":\"trigger\",\"sound\":1}},\"to\":\"dQfH_CVRXGQ:APA91bFadU6u8UHnaCfxeZqqndYT7L1WoiP9THmoD7SE2vxeuCakmNNpiJzv0dq9zPKum00hIcB0jjxsqvn31Dq_qyUlz0hV3lhyfmo-FIn2-8oJL7cUmEn589mRA5IYZf9I6tcTWDZy\"}";
header_map h;
header_value a;
a.sensitive = false;
a.value = "key=AIzaSyBss3ftDqPBR4StH3_eN_Nx6Eh_HThEaZ0";
h.insert(std::pair<std::string, header_value>("Authorization", a));
header_value b;
b.sensitive = false;
b.value="application/json";
h.insert(std::pair<std::string, header_value>("Content-Type", b));
for (std::size_t i = 0; i < num; ++i) {
auto req = sess.submit(ec, "POST",
"https://gcm-http.googleapis.com:443/gcm/send",data + std::to_string(i) + data1 , h);
if (req == NULL)
{
continue;
}
req->on_response(printer);
req->on_close([&sess, count](uint32_t error_code) {
if (--*count == 0) {
// shutdown session after |num| requests were done.
sess.shutdown();
}
});
}
});

sess.on_error([](const boost::system::error_code &ec) {
std::cerr << "error: " << ec.message() << std::endl;
});

io_service.run();
}
*/
