
#ifndef PHP_OGRENVELOPE_H
#define PHP_OGRENVELOPE_H

#include <ogr_feature.h>
#include "php.h"

extern zend_class_entry *gdal_ogrenvelope_ce;

struct php_ogrenvelope_object {
  zend_object std;
  OGREnvelope *envelope;
};

void php_gdal_ogrenvelope_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRENVELOPE_H */
