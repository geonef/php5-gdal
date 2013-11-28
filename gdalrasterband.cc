/* 
 * File:   gdalrasterband.cc
 * Author: Michal Schneider
 *
 * Created on November 28, 2013, 9:24 AM
 */

#include <gdal.h>
#include <gdal_priv.h>
#include "php.h"
#include "php_gdal.h"
#include "gdalrasterband.h"

zend_class_entry *gdal_gdalrasterband_ce;
zend_object_handlers gdalrasterband_object_handlers;

//
// PHP stuff
//

void gdaldriver_free_storage(void *object TSRMLS_DC)
{
  char *msg;
  int i;
  php_gdalrasterband_object *obj = (php_gdalrasterband_object *)object;

  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value gdalrasterband_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_gdalrasterband_object *obj =
    (php_gdalrasterband_object *)emalloc(sizeof(php_gdalrasterband_object));
  memset(obj, 0, sizeof(php_gdalrasterband_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
#if PHP_VERSION_ID < 50399
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref,
                 (void *)&tmp, sizeof(zval *));
#else
  object_properties_init(&obj->std, type);
#endif

  retval.handle =
    zend_objects_store_put(obj, NULL,
                           gdalrasterband_free_storage, NULL TSRMLS_CC);
  retval.handlers = &gdalrasterband_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

PHP_METHOD(GDALDataset, GetXSize)
{
  GDALRasterBand *rasterband;
  php_gdalrasterband_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdalrasterband_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  rasterband = obj->rasterband;
  RETURN_LONG(rasterband->GetXSize());
}

//
// PHP stuff
//

zend_function_entry gdalrasterband_methods[] = {
  PHP_ME(GDALRasterBand, GetXSize,                 NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_gdalrasterband_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "GDALRasterBand", gdalrasterband_methods);
  gdal_gdalrasterband_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_gdalrasterband_ce->create_object = gdalrasterband_create_handler;
  memcpy(&gdalrasterband_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  gdalrasterband_object_handlers.clone_obj = NULL;
}
