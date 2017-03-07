# apns2
apns2 gcm high-performance push module for php 5.6


		$a = new http20Client(); <br>
		$a->setHost("nghttp2.org"); <br>
		$a->setPort("443");<br>
		//$a->setPem("./cert.pem");<br>
		$d = $a->connectInit();<br>
		for($i=0; $i < 200; $i++)<br>
		{<br>
		  $a->connectExec("$i",<br>
		  array("body"=>"",<br>
		  "method"=>"GET",<br>
		  "url"=>"https://nghttp2.org/index.html",<br>
		  "headers"=>array("User-Agent"=>"http20Client")<br>
		 ));<br>
		}<br>
		$d = $a->waitResult();<br>
		$error = $a->getError();<br>
		print_r($d);<br>
		var_dump($error);<br>
		unset ($a);<br>
