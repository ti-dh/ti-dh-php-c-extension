/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dh.h"

/* If you declare any globals in php_dh.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dh)
*/

/* True global resources - no need for thread safety here */
static int le_dh;

#define MAXSIZE 1000
#include <gmp.h>
#include <sys/time.h>
#include <stdlib.h>

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dh.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dh_globals, dh_globals)
    STD_PHP_INI_ENTRY("dh.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dh_globals, dh_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_dh_compiled(string arg)
   Return a string to confirm that the module is compiled in */


PHP_FUNCTION( dh_init )
{
  //zend_string *strg;
  //strg = strpprintf( 0, "dh compute pga." );
  // 计算P
  char * p_source = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF"; 
  mpz_t p, g;
  mpz_init( p ); 
  mpz_init( g ); 
  mpz_init_set_str( p, p_source, 16 );
  // 计算G
  gmp_randstate_t state;
  gmp_randinit_default( state );
  gmp_randseed_ui( state, time( NULL ) ); 
  mpz_urandomm( g, state, p );
  /// 验证G是否合法
  int primitive_flag = 0;
  while ( !primitive_flag ) {
    mpz_t temp_p, temp_g, temp_sub, temp_flag;
    mpz_init( temp_p );
    mpz_init( temp_g );
    mpz_init( temp_sub );
    mpz_init( temp_flag );
    mpz_init_set_str( temp_sub,  "1", 10 );
    mpz_init_set_str( temp_flag, "1", 10 );
    // temp_p = p - temp_sub
    mpz_sub( temp_p, p, temp_sub );
    // temp_g = g ^ temp_p mod p
    mpz_powm( temp_g, g, temp_p, p ); 
    // 如果说这个g是可以使用的g
    if ( 0 == mpz_cmp( temp_g, temp_flag ) ) {
      primitive_flag = 1;
    } 
  }
  // 随机一个server_number
  mpz_t server_number;
  mpz_init( server_number );
  int oc_server_number;
  srand( time( NULL ) );
  oc_server_number = rand();
  mpz_set_ui( server_number, oc_server_number );
  // 好了，p、g、server_number都有了，下面可以计算给客户端的数字了 
  mpz_t processed_server_number;
  mpz_init( processed_server_number );
  mpz_powm( processed_server_number, g, server_number, p ); 
  //gmp_printf( "a ：%Zd\n", processed_server_number );
  // 构造返回
  char * p_str = mpz_get_str( NULL, 10, p ); 
  char * g_str = mpz_get_str( NULL, 10, g ); 
  char * a_str = mpz_get_str( NULL, 10, server_number ); 
  char * A_str = mpz_get_str( NULL, 10, processed_server_number ); 
  array_init( return_value );
  add_assoc_string( return_value, "p", p_str );
  add_assoc_string( return_value, "g", g_str );
  add_assoc_string( return_value, "server_number", a_str );
  add_assoc_string( return_value, "processed_server_number", A_str );
  // 释放内存
  mpz_clear( p ); 
  mpz_clear( g ); 
  mpz_clear( server_number ); 
  mpz_clear( processed_server_number ); 
  //RETURN_STR( strg );
}

/*
 * @desc : 计算出key
 */
PHP_FUNCTION( dh_compute_share_key )
{
	char * p_str;                //p
	size_t p_str_len;            //p
	char * a_str;                // server_number
	size_t a_str_len;            // server_number length
	char * client_num_str;       // client_number
	size_t client_num_str_len;   // client_number length
    //strg = strpprintf( 0, "dh compute key." );
    ZEND_PARSE_PARAMETERS_START( 3, 3 )
    Z_PARAM_STRING( client_num_str, client_num_str_len )
    Z_PARAM_STRING( a_str, a_str_len )
    Z_PARAM_STRING( p_str, p_str_len )
    ZEND_PARSE_PARAMETERS_END();

    // client_num有了，p和g也有，开始计算
    mpz_t key, client_num, p, a;
    mpz_init( client_num );
    mpz_init( p );
    mpz_init( a );
    mpz_init( key ); 
    // 给client_num p a数值
    mpz_init_set_str( p, p_str, 10 );
    mpz_init_set_str( a, a_str, 10 );
    mpz_init_set_str( client_num, client_num_str, 10 );
    //gmp_printf( "p ：%Zd\n", p );
    // key = B^a mod p
    mpz_powm( key, client_num, a, p ); 
    char * key_str = mpz_get_str( NULL, 10, key ); 
    // 释放内存
    mpz_clear( p );
    mpz_clear( a );
    mpz_clear( key );
    mpz_clear( client_num );
	RETURN_STRING( key_str );
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_dh_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_dh_init_globals(zend_dh_globals *dh_globals)
{
	dh_globals->global_value = 0;
	dh_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dh)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dh)
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
PHP_RINIT_FUNCTION(dh)
{
#if defined(COMPILE_DL_DH) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dh)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dh)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "dh support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ dh_functions[]
 *
 * Every user visible function must have an entry in dh_functions[].
 */
const zend_function_entry dh_functions[] = {
	PHP_FE( dh_init, NULL )		/* For testing, remove later. */
    PHP_FE( dh_compute_share_key, NULL )
	PHP_FE_END	/* Must be the last line in dh_functions[] */
};
/* }}} */

/* {{{ dh_module_entry
 */
zend_module_entry dh_module_entry = {
	STANDARD_MODULE_HEADER,
	"dh",
	dh_functions,
	PHP_MINIT(dh),
	PHP_MSHUTDOWN(dh),
	PHP_RINIT(dh),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dh),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dh),
	PHP_DH_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DH
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(dh)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
