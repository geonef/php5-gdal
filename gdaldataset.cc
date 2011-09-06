//
// gdaldataset.cc
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


#include <gdal.h>
#include <gdal_priv.h>
#include "php.h"
#include "php_gdal.h"
#include "gdaldataset.h"
#include "gdaldriver.h"

zend_class_entry *gdal_gdaldataset_ce;
zend_object_handlers gdaldataset_object_handlers;

//
// PHP stuff
//

void gdaldataset_free_storage(void *object TSRMLS_DC)
{
  char *msg;
  int i;
  php_gdaldataset_object *obj = (php_gdaldataset_object *)object;

  obj->dataset->Dereference();
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value gdaldataset_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_gdaldataset_object *obj =
    (php_gdaldataset_object *)emalloc(sizeof(php_gdaldataset_object));
  memset(obj, 0, sizeof(php_gdaldataset_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref,
                 (void *)&tmp, sizeof(zval *));

  retval.handle =
    zend_objects_store_put(obj, NULL,
                           gdaldataset_free_storage, NULL TSRMLS_CC);
  retval.handlers = &gdaldataset_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(GDALDataset, GetRasterXSize)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->GetRasterXSize());
}

PHP_METHOD(GDALDataset, GetRasterYSize)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->GetRasterYSize());
}

PHP_METHOD(GDALDataset, GetRasterCount)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->GetRasterCount());
}

PHP_METHOD(GDALDataset, FlushCache)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  dataset->FlushCache();
}

PHP_METHOD(GDALDataset, GetProjectionRef)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  const char *ref = dataset->GetProjectionRef();
  if (ref) {
    RETURN_STRING((char *)ref, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(GDALDataset, SetProjection)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;
  char *ref;
  int ref_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &ref, &ref_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->SetProjection(ref));
}

PHP_METHOD(GDALDataset, GetGeoTransform)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;
  double d;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"d",
                            &d) == FAILURE) {
    return;
  }
  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;

  RETURN_LONG(dataset->GetGeoTransform(&d));
}

PHP_METHOD(GDALDataset, SetGeoTransform)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;
  double d;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"d",
                            &d) == FAILURE) {
    return;
  }
  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;

  RETURN_LONG(dataset->SetGeoTransform(&d));
}

PHP_METHOD(GDALDataset, GetDriver)
{
  GDALDataset *dataset;
  GDALDriver *driver;
  php_gdaldataset_object *obj;
  php_gdaldriver_object *driver_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  driver = dataset->GetDriver();
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  driver_obj->driver = driver;
}

PHP_METHOD(GDALDataset, GetGCPCount)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->GetGCPCount());
}

PHP_METHOD(GDALDataset, CreateMaskBand)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;
  long nFlags;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &nFlags) == FAILURE) {
    return;
  }
  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;

  RETURN_LONG(dataset->CreateMaskBand(nFlags));
}

PHP_METHOD(GDALDataset, Reference)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->Reference());
}

PHP_METHOD(GDALDataset, Dereference)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->Dereference());
}

PHP_METHOD(GDALDataset, GetShared)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  RETURN_LONG(dataset->GetShared());
}

PHP_METHOD(GDALDataset, MarkAsShared)
{
  GDALDataset *dataset;
  php_gdaldataset_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldataset_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  dataset = obj->dataset;
  dataset->MarkAsShared();
}


//
// PHP stuff
//

function_entry gdaldataset_methods[] = {
  PHP_ME(GDALDataset, GetRasterXSize, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetRasterYSize, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetRasterCount, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetRasterBand, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, FlushCache, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetProjectionRef, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, SetProjection, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetGeoTransform, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, SetGeoTransform, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, AddBand, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetDriver, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetFileList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetGCPCount, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetGCPProjection, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetGCPs, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, SetGCPs, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, AdviseRead, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, CreateMaskBand, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, BeginAsyncReader, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, EndAsyncReader, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, RasterIO, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, Reference, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, Dereference, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetAccess, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, GetShared, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDataset, MarkAsShared, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDataset, GetOpenDatasets, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  // PHP_ME(GDALDataset, BuildOverviews, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_gdaldataset_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "GDALDataset", gdaldataset_methods);
  gdal_gdaldataset_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_gdaldataset_ce->create_object = gdaldataset_create_handler;
  memcpy(&gdaldataset_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  gdaldataset_object_handlers.clone_obj = NULL;
}
