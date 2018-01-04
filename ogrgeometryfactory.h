//
// ogrgeometry.h
//

#ifndef PHP_OGRGEOMETRYFACTORY_H
#define PHP_OGRGEOMETRYFACTORY_H

#include <ogr_geometry.h>
#include "php.h"

extern zend_class_entry *gdal_ogrgeometryfactory_ce;

struct php_ogrgeometryfactory_object {
  zend_object std;
  OGRGeometryFactory *geometryFactory;
};

void php_gdal_ogrgeometryfactory_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRGEOMETRYFACTORY_H */
