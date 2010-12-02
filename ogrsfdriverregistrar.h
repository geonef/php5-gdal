
#ifndef OGRSFDRIVERREGISTRAR_H
#define OGRSFDRIVERREGISTRAR_H

#include "php.h"

extern zend_class_entry *gdal_ogrsfdriverregistrar_ce;

void php_gdal_ogrsfdriverregistrar_startup(INIT_FUNC_ARGS);

#endif /* OGRSFDRIVERREGISTRAR_H */
