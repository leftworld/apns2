/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author:                                                              |
   +----------------------------------------------------------------------+
   */

/* $Id$ */

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>       
#include <unistd.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
//#include "/data/php5/include/php/main/php.h"
//#include "/data/php5/include/php/main/php_ini.h"
//#include "/data/php5/include/php/ext/standard/info.h"	
}
#include "php_asiohttp2.h"
#include "http2.h"

/* If you declare any globals in php_asiohttp2.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(asiohttp2)
   */

/* True global resources - no need for thread safety here */
static int le_asiohttp2;

/* {{{ PHP_INI
*/
/* Remove comments and fill if you need to have entries in php.ini
   PHP_INI_BEGIN()
   STD_PHP_INI_ENTRY("asiohttp2.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_asiohttp2_globals, asiohttp2_globals)
   STD_PHP_INI_ENTRY("asiohttp2.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_asiohttp2_globals, asiohttp2_globals)
   PHP_INI_END()
   */
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_asiohttp2_compiled(string arg)
   Return a string to confirm that the module is compiled in */

zend_object_handlers http2_object_handlers;

struct http2_object {
	zend_object std;
	http2 *h2;
};

void http2_free_storage(void *object TSRMLS_DC)
{
	http2_object *obj = (http2_object *)object;
	delete obj->h2; 
	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);
	efree(obj);
}

zend_object_value http2_create_handler(zend_class_entry *type TSRMLS_DC)
{

	zval *tmp;
	zend_object_value retval;

	http2_object *obj = (http2_object *)emalloc(sizeof(http2_object));
	memset(obj, 0, sizeof(http2_object));
	obj->std.ce = type;

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &type->properties_info,
			(copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL,
			http2_free_storage, NULL TSRMLS_CC);
	retval.handlers = &http2_object_handlers;

	return retval;
}

zend_class_entry * http2_ce;

PHP_METHOD(http20Client, __construct)
{
	http2 *h2 = NULL;
	zval *object = getThis();

	h2 = new http2();
	http2_object *obj = (http2_object *)zend_object_store_get_object(object TSRMLS_CC);
	obj->h2	= h2;
}

PHP_METHOD(http20Client, setHost)
{
	http2 *h2;
	char *arg = NULL;
	int arg_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;
	h2->setHost((const char*)arg);
}

PHP_METHOD(http20Client, setPort)
{
	http2 *h2;        
	char *arg = NULL; 
	int arg_len;      
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;       
	}                 
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;     
	h2->setPort((const char*)arg);
}

PHP_METHOD(http20Client, setPem)
{
	http2 *h2;
	char *arg = NULL;
	int arg_len;     
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;       
	}                 
	if ((access(arg, 0)) == -1 )
	{
		RETURN_NULL();
	}
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;     
	h2->setPem((const char*)arg);
}

PHP_METHOD(http20Client, connectInit)
{
	http2 *h2;
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;     
	int r = h2->init();
	RETURN_LONG(r);
}

PHP_METHOD(http20Client, connectExec)
{
	zval* z_array;
	char* event_id;
	int event_id_len;
	int count, i;
	zval **z_item;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &event_id, &event_id_len, &z_array) == FAILURE) {
		return;       
	}
	http2 *h2;
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;     
	count = zend_hash_num_elements(Z_ARRVAL_P(z_array));
	if (count == 0)
	{
		RETURN_NULL();
	}
	data_fild * data = new data_fild();
	data->id = event_id;
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(z_array));
	for (i = 0; i < count; i ++) {
		char* key;
		ulong idx;
		if (zend_hash_get_current_key(Z_ARRVAL_P(z_array), &key, &idx, 0) == HASH_KEY_IS_STRING) {
			zend_hash_get_current_data(Z_ARRVAL_P(z_array), (void **) &z_item);
			if (strcmp(key, "method") == 0)
			{
				data->method = Z_STRVAL_PP(z_item);
			}
			if (strcmp(key, "body") == 0)
			{
				data->body = Z_STRVAL_PP(z_item);
			}
			if (strcmp(key, "url") == 0)
			{
				data->url = Z_STRVAL_PP(z_item);
			}
			if (strcmp(key, "headers") == 0)
			{
				int header_len = zend_hash_num_elements(Z_ARRVAL_PP(z_item));
				int j = 0;
				zval ** h_item;
				for(j = 0; j < header_len; j++)
				{
					char *hkey;
					ulong hidx;
					if (zend_hash_get_current_key(Z_ARRVAL_PP(z_item), &hkey, &hidx, 0) == HASH_KEY_IS_STRING) {
						zend_hash_get_current_data(Z_ARRVAL_PP(z_item), (void**) &h_item);
						data->headers.insert(std::make_pair(key, Z_STRVAL_PP(h_item)));
						zend_hash_move_forward(Z_ARRVAL_PP(z_item));
					}
				}
			}
			zend_hash_move_forward(Z_ARRVAL_P(z_array));
		}
	}
	h2->add2ready(data);
}

PHP_METHOD(http20Client, waitResult)
{
	http2 *h2;
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;
	h2->wait_result();
	zval * ret;
	MAKE_STD_ZVAL(ret);
	array_init(ret);
	for(auto m : h2->result)
	{
		add_assoc_string(ret, (char*)m.first.c_str(), (char*)m.second.c_str(), 1);
	}
	RETURN_ZVAL(ret, 0, 1);
}

PHP_METHOD(http20Client, getError)
{
	http2 *h2;
	http2_object *obj = (http2_object *)zend_object_store_get_object(
			getThis() TSRMLS_CC);
	h2 = obj->h2;
	int len = h2->error.size();
	if (len == 0)
	{
		RETURN_NULL();
	}
	char* ret = (char*)emalloc(len);
	memcpy(ret, h2->error.c_str(), len);
	RETURN_STRINGL(ret, len, 0);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
   */


/* {{{ php_asiohttp2_init_globals
*/
/* Uncomment this function if you have INI entries
   static void php_asiohttp2_init_globals(zend_asiohttp2_globals *asiohttp2_globals)
   {
   asiohttp2_globals->global_value = 0;
   asiohttp2_globals->global_string = NULL;
   }
   */
/* }}} */

/* {{{ asiohttp2_functions[]
 *  *
 *   * Every user visible function must have an entry in asiohttp2_functions[].
 *    */
const zend_function_entry asiohttp2_functions[] = {
	//PHP_FE(confirm_asiohttp2_compiled,    NULL)       /* For testing, remove later. */
	PHP_ME(http20Client,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
		PHP_ME(http20Client,  setHost,           NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  setPort,      NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  setPem,           NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  connectInit, NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  connectExec,  NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  waitResult,  NULL, ZEND_ACC_PUBLIC)
		PHP_ME(http20Client,  getError,  NULL, ZEND_ACC_PUBLIC)
		PHP_FE_END  /* Must be the last line in asiohttp2_functions[] */
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(asiohttp2)
{
	/* If you have INI entries, uncomment these lines 
	   REGISTER_INI_ENTRIES();
	   */
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "http20Client", asiohttp2_functions);
	http2_ce = zend_register_internal_class(&ce TSRMLS_CC);
	http2_ce->create_object = http2_create_handler;
	memcpy(&http2_object_handlers,
			zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	http2_object_handlers.clone_obj = NULL;
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(asiohttp2)
{
	/* uncomment this line if you have INI entries
	   UNREGISTER_INI_ENTRIES();
	   */
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(asiohttp2)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(asiohttp2)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(asiohttp2)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "asiohttp2 support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	   DISPLAY_INI_ENTRIES();
	   */
}
/* }}} */


/* {{{ asiohttp2_module_entry
*/
zend_module_entry asiohttp2_module_entry = {
	STANDARD_MODULE_HEADER,
	"asiohttp2",
	asiohttp2_functions,
	PHP_MINIT(asiohttp2),
	PHP_MSHUTDOWN(asiohttp2),
	PHP_RINIT(asiohttp2),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(asiohttp2),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(asiohttp2),
	PHP_ASIOHTTP2_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ASIOHTTP2
extern "C" {
	ZEND_GET_MODULE(asiohttp2)
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
