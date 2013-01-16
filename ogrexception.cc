#include "php.h"
#include "php_gdal.h"
#include <zend_exceptions.h>
#include "ogrexception.h"

zend_class_entry *gdal_ogrexception_ce;
zend_object_handlers ogrexception_object_handlers;

function_entry ogrexception_methods[] = {
  {NULL, NULL, NULL}
};

void php_gdal_ogrexception_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRException", ogrexception_methods);
  gdal_ogrexception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
}

void php_gdal_ogr_throw_error_with_message(int error_code, char *error_message, int strict_error TSRMLS_DC)
{
  (void *) strict_error;
  zend_throw_exception(gdal_ogrexception_ce, error_message, error_code TSRMLS_CC);
}   

/* VIM settings */
/* ex: set tabstop=2 expandtab shiftwidth=2 smartindent */
