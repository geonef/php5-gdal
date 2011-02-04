

#include <gdal.h>
#include <gdal_priv.h>
#include "php.h"
#include "php_gdal.h"
#include "gdaldriver.h"
#include "gdaldataset.h"

zend_class_entry *gdal_gdaldriver_ce;
zend_object_handlers gdaldriver_object_handlers;

//
// PHP stuff
//

void gdaldriver_free_storage(void *object TSRMLS_DC)
{
  char *msg;
  int i;
  php_gdaldriver_object *obj = (php_gdaldriver_object *)object;

  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value gdaldriver_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_gdaldriver_object *obj =
    (php_gdaldriver_object *)emalloc(sizeof(php_gdaldriver_object));
  memset(obj, 0, sizeof(php_gdaldriver_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref,
                 (void *)&tmp, sizeof(zval *));

  retval.handle =
    zend_objects_store_put(obj, NULL,
                           gdaldriver_free_storage, NULL TSRMLS_CC);
  retval.handlers = &gdaldriver_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(GDALDriver, GetDescription)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  int name_len;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  const char *descr = driver->GetDescription();
  if (descr) {
    RETURN_STRING((char *)descr, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(GDALDriver, SetDescription)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  driver->SetDescription(name);
}

PHP_METHOD(GDALDriver, Delete)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->Delete(name));
}

PHP_METHOD(GDALDriver, Rename)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  char *name2;
  int name_len;
  int name2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"ss",
                            &name, &name_len, &name, &name_len)
      == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->Rename(name, name2));
}

PHP_METHOD(GDALDriver, CopyFiles)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  char *name2;
  int name_len;
  int name2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"ss",
                            &name, &name_len, &name, &name_len)
      == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->CopyFiles(name, name2));
}

PHP_METHOD(GDALDriver, DefaultCopyMasks)
{
  GDALDriver *driver;
  GDALDataset *dataset1;
  GDALDataset *dataset2;
  php_gdaldriver_object *obj;
  php_gdaldataset_object *dataset1_obj;
  php_gdaldataset_object *dataset2_obj;
  zval *dataset1_p;
  zval *dataset2_p;
  int bStrict;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"OOl",
                            &dataset1_p, gdal_gdaldataset_ce,
                            &dataset1_p, gdal_gdaldataset_ce,
                            &bStrict) == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;

  dataset1_obj = (php_gdaldataset_object*)
    zend_object_store_get_object(dataset1_p);
  dataset1 = dataset1_obj->dataset;
  dataset2_obj = (php_gdaldataset_object*)
    zend_object_store_get_object(dataset2_p);
  dataset2 = dataset2_obj->dataset;

  RETURN_LONG(driver->DefaultCopyMasks(dataset1, dataset2, bStrict));
}

PHP_METHOD(GDALDriver, QuietDelete)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->QuietDelete(name));
}

PHP_METHOD(GDALDriver, DefaultRename)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  char *name2;
  int name_len;
  int name2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"ss",
                            &name, &name_len, &name, &name_len)
      == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->DefaultRename(name, name2));
}

PHP_METHOD(GDALDriver, DefaultCopyFiles)
{
  GDALDriver *driver;
  php_gdaldriver_object *obj;
  char *name;
  char *name2;
  int name_len;
  int name2_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"ss",
                            &name, &name_len, &name, &name_len)
      == FAILURE) {
    return;
  }

  obj = (php_gdaldriver_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_LONG(driver->DefaultCopyFiles(name, name2));
}

//
// PHP stuff
//

function_entry gdaldriver_methods[] = {
  //-- parent class
  PHP_ME(GDALDriver, GetDescription, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, SetDescription, NULL, ZEND_ACC_PUBLIC)
  //--
  // PHP_ME(GDALDriver, Create, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, Delete, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, Rename, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, CopyFiles, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDriver, CreateCopy, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(GDALDriver, DefaultCreateCopy, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, DefaultCopyMasks, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, QuietDelete, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, DefaultRename, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriver, DefaultCopyFiles, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_gdaldriver_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "GDALDriver", gdaldriver_methods);
  gdal_gdaldriver_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_gdaldriver_ce->create_object = gdaldriver_create_handler;
  memcpy(&gdaldriver_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  gdaldriver_object_handlers.clone_obj = NULL;
}
