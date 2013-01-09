/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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
#include "php_resvsobj.h"

/* {{{ resource-based implementation */
static int le_resvsobj;

#define RESVSOBJ_RES_NAME "resource vs object - resource"

static void resvsobj_res_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    efree(rsrc->ptr);
}

PHP_FUNCTION(resvsobj_new_resource)
{
	long *value = emalloc(sizeof(long));
	*value = 0;
	ZEND_REGISTER_RESOURCE(return_value, value, le_resvsobj);
}

PHP_FUNCTION(resvsobj_add)
{
	long *value;
	zval *value_res;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &value_res) == FAILURE) {
		return;
	}
	ZEND_FETCH_RESOURCE(value, long *, &value_res, -1, RESVSOBJ_RES_NAME, le_resvsobj);
	(*value)++;
}

PHP_FUNCTION(resvsobj_get_value)
{
	long *value;
	zval *value_res;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &value_res) == FAILURE) {
		return;
	}
	ZEND_FETCH_RESOURCE(value, long *, &value_res, -1, RESVSOBJ_RES_NAME, le_resvsobj);
	RETURN_LONG(*value);
}

const zend_function_entry resvsobj_functions[] = {
	PHP_FE(resvsobj_new_resource,	NULL)
	PHP_FE(resvsobj_add,	NULL)
	PHP_FE(resvsobj_get_value,	NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ object-based implementation */
static zend_class_entry *resvsobj_ce;

typedef struct {
	zend_object zo;
	long value;
} refvsobj_object;

zend_object_value resvsobj_object_new(zend_class_entry *ce TSRMLS_DC) {
	zend_object_value retval;
	refvsobj_object *intern;
	intern = ecalloc(1, sizeof(refvsobj_object));
	zend_object_std_init(&(intern->zo), ce TSRMLS_CC);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, (zend_objects_free_object_storage_t) zend_objects_free_object_storage, NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();

	return retval;
}

PHP_METHOD(resvsobj, __construct)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
}

PHP_METHOD(resvsobj, add)
{
	zval *object = getThis();
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	refvsobj_object *intern = (refvsobj_object*)zend_object_store_get_object(object TSRMLS_CC);
	intern->value++;
}

PHP_METHOD(resvsobj, getValue)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	zval *object = getThis();
	refvsobj_object *intern = (refvsobj_object*)zend_object_store_get_object(object TSRMLS_CC);
	RETURN_LONG(intern->value);
}

const zend_function_entry resvsobj_methods[] = {
	PHP_ME(resvsobj, __construct,	NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(resvsobj, add,	NULL, ZEND_ACC_PUBLIC)
	PHP_ME(resvsobj, getValue,	NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(resvsobj)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "resvsobj", resvsobj_methods);
	resvsobj_ce = zend_register_internal_class(&ce TSRMLS_CC);
        resvsobj_ce->create_object = resvsobj_object_new;

	le_resvsobj = zend_register_list_destructors_ex(resvsobj_res_dtor, NULL, RESVSOBJ_RES_NAME, module_number);


	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(resvsobj)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(resvsobj)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "resvsobj support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ resvsobj_module_entry
 */
zend_module_entry resvsobj_module_entry = {
	STANDARD_MODULE_HEADER,
	"resvsobj",
	resvsobj_functions,
	PHP_MINIT(resvsobj),
	PHP_MSHUTDOWN(resvsobj),
	NULL,
	NULL,
	PHP_MINFO(resvsobj),
	"0.1",
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RESVSOBJ
ZEND_GET_MODULE(resvsobj)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
