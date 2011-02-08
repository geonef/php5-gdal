
#ifndef PHP_OGRDATASOURCE_H
#define PHP_OGRDATASOURCE_H

#include <ogrsf_frmts.h>
#include "php.h"

extern zend_class_entry *gdal_ogrdatasource_ce;

struct php_ogrdatasource_object {
  zend_object std;
  OGRDataSource *datasource;
  ulong hashIndex;
};

void php_gdal_ogrdatasource_startup(INIT_FUNC_ARGS);
void php_gdal_ogrdatasource_add_to_hash(php_ogrdatasource_object *obj);
void php_gdal_ogrdatasource_ptr_dtor(void **ptr);
//void ogrdatasource_destroy_all();

#endif /* PHP_OGRDATASOURCE_H */
