/* 
 * File:   gdalrasterband.h
 * Author: Michal Schneider
 *
 * Created on November 28, 2013, 9:24 AM
 */

#ifndef PHP_GDALRASTERBAND_H
#define PHP_GDALRASTERBAND_H

#include <gdal_priv.h>
#include "php.h"

extern zend_class_entry *gdal_gdalrasterband_ce;

struct php_gdalrasterband_object {
  zend_object std;
  GDALRasterBand *rasterband;
};

void php_gdal_gdalrasterband_startup(INIT_FUNC_ARGS);

#endif /* PHP_GDALRASTERBAND_H */

