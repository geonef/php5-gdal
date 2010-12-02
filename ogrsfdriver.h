
#ifndef OGRSFDRIVER_H
#define OGRSFDRIVER_H

#include "php.h"

extern zend_class_entry *gdal_ogrsfdriver_ce;

struct driver_object {
  zend_object std;
  OGRSFDriver *driver;
};

void php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS);

#endif /* OGRSFDRIVER_H */
