//
// gdal_main.cc
//
//
// Copyright (c) 2011, Geonef SAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "php.h"
#include <gdal.h>
#include <gdal_priv.h>
#include "gdal_main.h"
#include "gdaldataset.h"
#include "gdaldrivermanager.h"

#ifndef GDAL_H_INCLUDED
#error "grrr"
#endif

/**
 *
 * "VERSION_NUM", "RELEASE_DATE", "RELEASE_NAME", "--version", "LICENCE"
 */
PHP_FUNCTION(gdalversioninfo)
{
  char *request;
  int request_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &request, &request_len) == FAILURE) {
    WRONG_PARAM_COUNT;
  }
  const char *info = GDALVersionInfo(request);
  if (info) {
    RETURN_STRING((char*)info, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_FUNCTION(gdalopen)
{
  GDALDataset *dataset;
  php_gdaldataset_object *dataset_obj;
  char *path;
  int path_len;
  int mode;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"sl",
                            &path, &path_len, &mode) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  dataset = (GDALDataset *) GDALOpen(path, mode == GA_ReadOnly ?
                                     GA_ReadOnly : GA_Update);
  if (!dataset) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldataset_ce) != SUCCESS) {
    RETURN_NULL();
  }
  dataset_obj = (php_gdaldataset_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  dataset_obj->dataset = dataset;

}

PHP_FUNCTION(gdalopenshared)
{
  GDALDataset *dataset;
  php_gdaldataset_object *dataset_obj;
  char *path;
  int path_len;
  int mode;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"sl",
                            &path, &path_len, &mode) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  dataset = (GDALDataset *) GDALOpenShared(path, mode == GA_ReadOnly ?
                                           GA_ReadOnly : GA_Update);
  if (!dataset) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldataset_ce) != SUCCESS) {
    RETURN_NULL();
  }
  dataset_obj = (php_gdaldataset_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  dataset_obj->dataset = dataset;

}

PHP_FUNCTION(getgdaldrivermanager)
{
  GDALDriverManager *manager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  manager = GetGDALDriverManager();
  if (!manager) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldrivermanager_ce) != SUCCESS) {
    RETURN_NULL();
  }
  obj = (php_gdaldrivermanager_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  obj->drivermanager = manager;

}

PHP_FUNCTION(gdalallregister)
{
  GDALDriverManager *manager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  GDALAllRegister();
}
