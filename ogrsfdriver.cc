

#include "php.h"
#include "php_gdal.h"
#include "ogrsf_frmts.h"
#include "ogrsfdriver.h"

zend_class_entry *gdal_ogrsfdriver_ce;
zend_object_handlers driver_object_handlers;

//
// PHP stuff
//

void ogrsfdriver_free_storage(void *object TSRMLS_DC)
{
  driver_object *obj = (driver_object *)object;
  //delete obj->layer; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrsfdriver_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  driver_object *obj = (driver_object *)emalloc(sizeof(driver_object));
  memset(obj, 0, sizeof(driver_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrsfdriver_free_storage, NULL TSRMLS_CC);
  retval.handlers = &driver_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRSFDriver, GetName)
{
  OGRSFDriver *driver;
  driver_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (driver_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  const char *name = driver->GetName();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRSFDriver, TestCapability)
{
  OGRSFDriver *driver;
  driver_object *obj;
  char *strcapability = NULL;
  int strcapability_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &strcapability, &strcapability_len) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (driver_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  driver = obj->driver;
  RETURN_BOOL(driver->TestCapability(strcapability));
}


//
// PHP stuff
//

function_entry ogrsfdriver_methods[] = {
  PHP_ME(OGRSFDriver, GetName, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver, Open, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriver, TestCapability, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver, CreateDataSource, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver, DeleteDataSource, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver, CopyDataSource, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRSFDriver", ogrsfdriver_methods);
  gdal_ogrsfdriver_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrsfdriver_ce->create_object = ogrsfdriver_create_handler;
  memcpy(&driver_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  driver_object_handlers.clone_obj = NULL;
}
