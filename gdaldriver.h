
#ifndef PHP_GDALDRIVER_H
#define PHP_GDALDRIVER_H

#include <gdal_priv.h>
#include "php.h"

extern zend_class_entry *gdal_gdaldriver_ce;

struct php_gdaldriver_object {
  zend_object std;
  GDALDriver *driver;
};

void php_gdal_gdaldriver_startup(INIT_FUNC_ARGS);

#endif /* PHP_GDALDRIVER_H */
