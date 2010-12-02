
#ifndef PHP_OGRLAYER_H
#define PHP_OGRLAYER_H

#include <ogrsf_frmts.h>
#include "php.h"

extern zend_class_entry *gdal_ogrlayer_ce;

struct php_ogrlayer_object {
  zend_object std;
  OGRLayer *layer;
};

void php_gdal_ogrlayer_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRLAYER_H */
