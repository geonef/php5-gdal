
#ifndef PHP_OGRDATASOURCE_H
#define PHP_OGRDATASOURCE_H

#include <ogrsf_frmts.h>
#include "php.h"

extern zend_class_entry *gdal_ogrdatasource_ce;

struct php_ogrdatasource_object {
  zend_object std;
  OGRDataSource *datasource;
};

void php_gdal_ogrdatasource_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRDATASOURCE_H */
