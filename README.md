
#apns2
	apns2 gcm high-performance push module for php 5.6 
	base on libnghttp2 https://nghttp2.org  
	high-performance over a million per minute!

#Installation
	1. Install libnghttp2 :
		 autoreconf -i
		 automake
		 autoconf
		 configure --enable-asio-lib
		 make
		 make install
	2. Install asiohttp2:
		 cd apns2 current path
		 phpize
		 ./configure --enable-asiohttp2
		 make
		 make install
			
#example
```php
		$a = new http20Client(); 
		$a->setHost("nghttp2.org"); 
		$a->setPort("443");
		//$a->setPem("./cert.pem");
		$d = $a->connectInit();
		for($i=0; $i < 200; $i++)
		{
		  $a->connectExec("$i",
		  array("body"=>"",
		  "method"=>"GET",
		  "url"=>"https://nghttp2.org/index.html",
		  "headers"=>array("User-Agent"=>"http20Client")
		 ));
		}
		$d = $a->waitResult();
		$error = $a->getError();
		print_r($d);
		var_dump($error);
		unset ($a);
```
