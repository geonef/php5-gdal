
#ifndef PHP_OGRFIELDDEFN_H
#define PHP_OGRFIELDDEFN_H

#include <ogr_feature.h>
#include "php.h"

extern zend_class_entry *gdal_ogrfielddefn_ce;

struct php_ogrfielddefn_object {
  zend_object std;
  OGRFieldDefn *fielddefn;
};

void php_gdal_ogrfielddefn_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRFIELDDEFN_H */
