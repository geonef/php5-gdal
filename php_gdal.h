#ifndef PHP_GDAL_H
#define PHP_GDAL_H

#define PHP_GDAL_EXTNAME  "gdal"
#define PHP_GDAL_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

PHP_FUNCTION(gdal_test);

extern zend_module_entry gdal_module_entry;
#define phpext_gdal_ptr &gdal_module_entry;

#endif /* PHP_GDAL_H */
