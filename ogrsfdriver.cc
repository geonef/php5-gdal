

#include "php.h"
#include "php_gdal.h"
#include "ogrsf_frmts.h"
#include "ogrsfdriver.h"

zend_class_entry *gdal_ogrsfdriver_ce;
zend_object_handlers registrar_object_handlers;

struct registrar_object {
  zend_object std;
  OGRSFDriver *driver;
};

//
// PHP stuff
//

void ogrsfdriver_free_storage(void *object TSRMLS_DC)
{
  registrar_object *obj = (registrar_object *)object;
  //delete obj->layer; // singleton object - only the PHP obj is destroyed
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrsfdriver_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  registrar_object *obj = (registrar_object *)emalloc(sizeof(registrar_object));
  memset(obj, 0, sizeof(registrar_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrsfdriver_free_storage, NULL TSRMLS_CC);
  retval.handlers = &registrar_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

// zval *php_gdal_ogrsfdriver_wrap(OGRSFDriver *registrar)
// {
//   zval *zval;
//   registrar_object *obj;
//   zend_object_value val;
//   MAKE_STD_ZVAL(zval);

//   //val = ogrsfdriver_create_handler(gdal_ogrsfdriver_ce);
//   if (object_init_ex(zval, gdal_ogrsfdriver_ce) != SUCCESS) {
//     RETURN_NULL();
//   }
//   obj = (registrar_object*) zend_object_store_get_object(zval TSRMLS_CC);
//   obj->registrar = registrar;
//   return zval
// }

//
// Object methods
//

PHP_METHOD(OGRSFDriver, GetDriverCount)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriver *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  RETURN_LONG(registrar->GetDriverCount());
}

PHP_METHOD(OGRSFDriver, GetOpenDSCount)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriver *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  RETURN_LONG(registrar->GetOpenDSCount());
}

PHP_METHOD(OGRSFDriver, AutoLoadDrivers)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriver *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  registrar->AutoLoadDrivers();
}

PHP_METHOD(OGRSFDriver, GetRegistrar)
{
  OGRSFDriver *registrar;
  registrar_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  registrar = OGRSFDriver::GetRegistrar();

  if (object_init_ex(return_value, gdal_ogrsfdriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  obj = (registrar_object*) zend_object_store_get_object(return_value TSRMLS_CC);
  obj->registrar = registrar;

  //zval = php_gdal_ogrsfdriver_wrap(registrar);
  //RETURN_ZVAL(zval, 0, ogrsfdriver_free_storage);
}


//
// PHP stuff
//

function_entry ogrsfdriver_methods[] = {
  //PHP_ME(OGRSFDriver,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  //PHP_ME(OGRSFDriver,  RegisterDriver, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver,  DeregisterDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriver,  GetDriverCount, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver,  GetDriver,      NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver,  GetDriverByName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriver,  GetOpenDSCount, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRSFDriver,  GetOpenDS,      NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriver,  AutoLoadDrivers, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriver,  GetRegistrar,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  //PHP_ME(OGRSFDriver,  Open,           NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRSFDriver", ogrsfdriver_methods);
  gdal_ogrsfdriver_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrsfdriver_ce->create_object = ogrsfdriver_create_handler;
  memcpy(&registrar_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  registrar_object_handlers.clone_obj = NULL;
}
