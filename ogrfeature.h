
#ifndef PHP_OGRFEATURE_H
#define PHP_OGRFEATURE_H

#include <ogr_spatialref.h>
#include "php.h"

extern zend_class_entry *gdal_ogrfeature_ce;

struct php_ogrfeature_object {
  zend_object std;
  OGRFeature *feature;
};

void php_gdal_ogrfeature_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRFEATURE_H */
