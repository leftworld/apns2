
#apns2

	apns2 gcm high-performance concurrence push module for php 5.6 
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

		 test: php asiohttp2.php 

			
#example
```php
		<?php
		$a = new http20Client(); 
		$a->setHost("nghttp2.org"); 
		$a->setPort("443");
		//$a->setPem("./cert.pem"); //apple apns2 reqiured ,google not reqiured
		//$a->setPass("pass"); //support certificate password verification for apple
		$d = $a->connectInit();
		for($i=0; $i < 10; $i++) //$i less than 10000 a loop.
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

#attention

		class "http20Client" Every time works needs to be created. one "http20Client" object Can be used only once. After use "unset" destroy object.
