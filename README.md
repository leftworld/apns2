
#apns2

	apns2 gcm high-performance push module for php 5.6 
	base on libnghttp2 https://nghttp2.org  
	high-performance over a million per minute!

#Installation

	1. Install libnghttp2 : require libs: boost_system ssl crypto

		 visit to https://github.com/nghttp2/nghttp2 and clone it.

		 autoreconf -i

		 automake

		 autoconf

		 configure --enable-asio-lib

		 make

		 sudo make install

		 sudo ldconfig

	2. Install asiohttp2:

		 cd apns2 current path

		 phpize

		 ./configure --enable-asiohttp2

		 make

		 sudo make install
	
		 add "extension=asiohttp2.so" to your php.ini
			
#example
```php
		<?php
		$a = new http20Client(); 
		$a->setHost("nghttp2.org"); 
		$a->setPort("443");
		//$a->setPem("./cert.pem");
    //$a->setPass("pass"); //support certificate password verification
		$d = $a->connectInit();
		for($i=0; $i < 10; $i++)
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
		?>
```
