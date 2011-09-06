//
// gdal_main.cc
//
//
// Copyright (c) 2011, JF Gigand
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
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
