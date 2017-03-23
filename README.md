
# apns2

	HTTP/2 Apple Push Notification Service (APNs) push provider  
	Google Cloud Messaging (HTTP/2) (GCM)
	high-performance concurrence push module for php 5.6 
	base on libnghttp2 https://nghttp2.org  
	high-performance over a million per minute!

# Installation

	1. Install libnghttp2 : require libs: boost_system ssl crypto

		 visit to https://github.com/nghttp2/nghttp2 and clone it.

		 autoreconf -i

		 automake

		 autoconf

		 ./configure --enable-asio-lib

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

			
# example
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

# attention

		class "http20Client" Every time works needs to be created. 
		one "http20Client" object Can be used only once. 
		After use "unset" destroy object.
		

# multiprocess example


```php
<?php

$config = [
	"api.push.apple.com" => ["port"=>443, "pem"=>"./a.pem", "pass"=> "pass"]
];

class data_struct{
	public $host;
	public $body;
	public $method;
	public $url;
	public $headers = [];
}

$process_num = 16;

for($i = 0; $i < $process_num; $i++)
{
	$pid = pcntl_fork();
	if ($pid == -1)
	{
		die("error");
	}
	if ($pid == 0)
	{
		todo();
		exit;	
	}
}
$status;
pcntl_wait($status);

function todo()
{
	$redis = new redis_client();
	$redes->connect("127.0.0.1",6379);
	$send_buff = array();
	for(;;)
	{
		$data = $redis->lpop("key");
		if ($data == NULL)
		{
			if (count($send_buff) == 0)
			{
				$data = $redis->blpop("key");
				if (!isset($send_buff[$data->host]))
				{
					$send_buff[$data->host] = [];
				}
				$send_buff[$data->host][] = $data;
			}else{
				process_data($send_buff);
				$send_buff = [];
			}
		}else{
			if (!isset($send_buff[$data->host]))
			{
				$send_buff[$data->host] = [];
			}
			$send_buff[$data->host][] = $data;
			$i = 0;
			foreach($send_buff as $k => $v)
			{
				$i += count($v);	
			}
			if ($i >= 10000)
			{
				process_data($send_buff);
				$send_buff = [];
			}
		}
	}
}

function process_data(&$send_buff)
{
	foreach($send_buff as $k => $v)
	{        
		$result = send_data($k, $v);
		foreach($v as $key => $val)
		{
			if (isset($result[$key]) && $result[$key]["code"] == 200)
			{   
				//send_ok       
			}else{
				process_error($v);
			}
		}
	}
}


function process_error($arr)
{
	//todo
}

function send_data($host, $data)
{
	global $config;
	$h2 = new http20Client(); 
	$h2->setHost("$host"); 
	$h2->setPort($config["$host"]["port"]);
	if (isset($config["$host"]["pem"]))
	{
		$h2->setPem($config["$host"]["pem"]); //apple apns2 reqiured ,google not reqiured
	}
	if (isset($config["$host"]["pass"]))
	{
		$h2->setPass($config["$host"]["pass"]); //support certificate password verification for apple
	}
	$h2->connectInit();
	foreach($data as $k => $v)
	{
		$h2->connectExec("$k", $v);
	}
	$result = $h2->waitResult();
	unset ($h2);
	return $result;
}
?>
```
