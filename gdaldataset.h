
#ifndef PHP_GDALDATASET_H
#define PHP_GDALDATASET_H

#include <gdal_priv.h>
#include "php.h"

extern zend_class_entry *gdal_gdaldataset_ce;

struct php_gdaldataset_object {
  zend_object std;
  GDALDataset *dataset;
};

void php_gdal_gdaldataset_startup(INIT_FUNC_ARGS);

#endif /* PHP_GDALDATASET_H */
