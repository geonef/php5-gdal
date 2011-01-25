
#ifndef PHP_OGRCOORDTRANSFORM_H
#define PHP_OGRCOORDTRANSFORM_H

#include <ogr_spatialref.h>
#include "php.h"

extern zend_class_entry *gdal_ogrcoordtransform_ce;

struct php_ogrcoordtransform_object {
  zend_object std;
  OGRCoordinateTransformation *coordtransform;
};

void php_gdal_ogrcoordtransform_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRCOORDTRANSFORM_H */
