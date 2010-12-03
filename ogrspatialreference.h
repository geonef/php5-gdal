
#ifndef PHP_OGRSPATIALREFERENCE_H
#define PHP_OGRSPATIALREFERENCE_H

#include <ogr_spatialref.h>
#include "php.h"

extern zend_class_entry *gdal_ogrspatialreference_ce;

struct php_ogrspatialreference_object {
  zend_object std;
  OGRSpatialReference *spatialreference;
};

void php_gdal_ogrspatialreference_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRSPATIALREFERENCE_H */
