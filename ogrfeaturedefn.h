
#ifndef PHP_OGRFEATUREDEFN_H
#define PHP_OGRFEATUREDEFN_H

#include <ogr_feature.h>
#include "php.h"

extern zend_class_entry *gdal_ogrfeaturedefn_ce;

struct php_ogrfeaturedefn_object {
  zend_object std;
  OGRFeatureDefn *featuredefn;
};

void php_gdal_ogrfeaturedefn_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRFEATUREDEFN_H */
