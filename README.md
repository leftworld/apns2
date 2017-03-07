
#apns2
	apns2 gcm high-performance push module for php 5.6 
	base on libnghttp2 https://nghttp2.org  
	high-performance over a million per minute!

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
