//
// ogrexception.h
//

#ifndef PHP_OGREXCEPTION_H
#define PHP_OGREXCEPTION_H

#include "php.h"

extern zend_class_entry *gdal_ogrexception_ce;

struct php_ogrexception_object {
  zend_object std;
};

void php_gdal_ogrexception_startup(INIT_FUNC_ARGS);

void php_gdal_ogr_throw_error_with_message(int error_code, char *error_message, int strict_error TSRMLS_DC);

#endif /* PHP_OGREXCEPTION_H */
