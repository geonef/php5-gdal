
#ifndef PHP_GDALDRIVERMANAGER_H
#define PHP_GDALDRIVERMANAGER_H

#include <gdal_priv.h>
#include "php.h"

extern zend_class_entry *gdal_gdaldrivermanager_ce;

struct php_gdaldrivermanager_object {
  zend_object std;
  GDALDriverManager *drivermanager;
};

void php_gdal_gdaldrivermanager_startup(INIT_FUNC_ARGS);

#endif /* PHP_GDALDRIVERMANAGER_H */
