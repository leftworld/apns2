<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('asiohttp2')) {
	dl('asiohttp2.' . PHP_SHLIB_SUFFIX);
}
$module = 'asiohttp2';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
?>
