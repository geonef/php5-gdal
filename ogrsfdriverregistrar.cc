//
// ogrsfdriverregistrar.cc
//
//
// Copyright (c) 2011, Geonef SAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include <ogrsf_frmts.h>
#include "php.h"
#include "php_gdal.h"
#include "ogrsfdriverregistrar.h"
#include "ogrsfdriver.h"
#include "ogrdatasource.h"

zend_class_entry *gdal_ogrsfdriverregistrar_ce;
zend_object_handlers registrar_object_handlers;

struct registrar_object {
  zend_object std;
  OGRSFDriverRegistrar *registrar;
};

//
// PHP stuff
//

void ogrsfdriverregistrar_free_storage(void *object TSRMLS_DC)
{
  registrar_object *obj = (registrar_object *)object;
  //delete obj->layer; // singleton object - only the PHP obj is destroyed
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrsfdriverregistrar_create_handler(zend_class_entry *type TSRMLS_DC)
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
                                         ogrsfdriverregistrar_free_storage, NULL TSRMLS_CC);
  retval.handlers = &registrar_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

// zval *php_gdal_ogrsfdriverregistrar_wrap(OGRSFDriverRegistrar *registrar)
// {
//   zval *zval;
//   registrar_object *obj;
//   zend_object_value val;
//   MAKE_STD_ZVAL(zval);

//   //val = ogrsfdriverregistrar_create_handler(gdal_ogrsfdriverregistrar_ce);
//   if (object_init_ex(zval, gdal_ogrsfdriverregistrar_ce) != SUCCESS) {
//     RETURN_NULL();
//   }
//   obj = (registrar_object*) zend_object_store_get_object(zval TSRMLS_CC);
//   obj->registrar = registrar;
//   return zval
// }

//
// Object methods
//

PHP_METHOD(OGRSFDriverRegistrar, RegisterDriver)
{
  // TODO: check object type
  OGRSFDriverRegistrar *registrar;
  OGRSFDriver *driver;
  registrar_object *obj;
  driver_object *driverObj;
  zval *drvp;
  zend_class_entry *ce = NULL;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &drvp, gdal_ogrsfdriver_ce) == FAILURE) {
    return;
  }

  obj = (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  driverObj = (driver_object *)zend_object_store_get_object(drvp);
  if (driverObj) {
    driver = driverObj->driver;
    registrar->RegisterDriver(driver);
  }
}

PHP_METHOD(OGRSFDriverRegistrar, GetDriverCount)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriverRegistrar *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  RETURN_LONG(registrar->GetDriverCount());
}

PHP_METHOD(OGRSFDriverRegistrar, GetDriver)
{
  OGRSFDriverRegistrar *registrar;
  OGRSFDriver *driver;
  registrar_object *obj;
  driver_object *driverObj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l", &index) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;

  driver = registrar->GetDriver(index);
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrsfdriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driverObj = (driver_object*) zend_object_store_get_object(return_value TSRMLS_CC);
  driverObj->driver = driver;
}

PHP_METHOD(OGRSFDriverRegistrar, GetDriverByName)
{
  OGRSFDriverRegistrar *registrar;
  OGRSFDriver *driver;
  registrar_object *obj;
  driver_object *driverObj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;

  driver = registrar->GetDriverByName(name);
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrsfdriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driverObj = (driver_object*) zend_object_store_get_object(return_value TSRMLS_CC);
  driverObj->driver = driver;
}

PHP_METHOD(OGRSFDriverRegistrar, GetOpenDSCount)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriverRegistrar *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  RETURN_LONG(registrar->GetOpenDSCount());
}

PHP_METHOD(OGRSFDriverRegistrar, GetOpenDS)
{
  OGRSFDriverRegistrar *registrar;
  registrar_object *obj;
  OGRDataSource *datasource;
  php_ogrdatasource_object *datasource_obj;
  long index;

  // TODO: &$driver param
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l", &index)
      == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  datasource = registrar->GetOpenDS(index);

  if (!datasource) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrdatasource_ce) != SUCCESS) {
    RETURN_NULL();
  }
  datasource_obj = (php_ogrdatasource_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  datasource_obj->datasource = datasource;
}

PHP_METHOD(OGRSFDriverRegistrar, AutoLoadDrivers)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  OGRSFDriverRegistrar *registrar;
  registrar_object *obj =
    (registrar_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
  registrar = obj->registrar;
  registrar->AutoLoadDrivers();
}

PHP_METHOD(OGRSFDriverRegistrar, GetRegistrar)
{
  OGRSFDriverRegistrar *registrar;
  registrar_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  registrar = OGRSFDriverRegistrar::GetRegistrar();

  if (object_init_ex(return_value, gdal_ogrsfdriverregistrar_ce) != SUCCESS) {
    RETURN_NULL();
  }
  obj = (registrar_object*) zend_object_store_get_object(return_value TSRMLS_CC);
  obj->registrar = registrar;

  //zval = php_gdal_ogrsfdriverregistrar_wrap(registrar);
  //RETURN_ZVAL(zval, 0, ogrsfdriverregistrar_free_storage);
}

PHP_METHOD(OGRSFDriverRegistrar, Open)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *datasource_obj;
  char *ds_name;
  int ds_name_len;
  zend_bool update_mode = 0;
  //zval **zdrv;

  // TODO: &$driver param
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s|b",
                            &ds_name, &ds_name_len, &update_mode
                            /*&zdrv*/) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  if (ds_name_len == 0) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty string as path");
    RETURN_FALSE;
  }

  datasource = OGRSFDriverRegistrar::Open(ds_name, update_mode, NULL);
  if (!datasource) {
    RETURN_NULL();
  }
  ////
  // char *msg; int i;
  // i = datasource->GetRefCount();
  // asprintf(&msg, "OGRSFDriverRegistrar::Open path=\"%s\" refcount=%d", ds_name, i);
  // php_log_err(msg);
  // free(msg);
  ////
  if (object_init_ex(return_value, gdal_ogrdatasource_ce) != SUCCESS) {
    RETURN_NULL();
  }
  datasource_obj = (php_ogrdatasource_object*)
    //zend_object_store_get_object
    zend_objects_get_address(return_value TSRMLS_CC);
  datasource_obj->datasource = datasource;

  php_gdal_ogrdatasource_add_to_hash(datasource_obj);
}

//
// PHP stuff
//

function_entry ogrsfdriverregistrar_methods[] = {
  // despite the API web page, there is no DeregisterDriver()
  PHP_ME(OGRSFDriverRegistrar,  RegisterDriver,  NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetDriverCount,  NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetDriver,       NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetDriverByName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetOpenDSCount,  NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetOpenDS,       NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  AutoLoadDrivers, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSFDriverRegistrar,  GetRegistrar,    NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(OGRSFDriverRegistrar,  Open,            NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  // We don't use OpenShared, to keep things simple
  //PHP_ME(OGRSFDriverRegistrar,  OpenShared,       NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrsfdriverregistrar_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRSFDriverRegistrar", ogrsfdriverregistrar_methods);
  gdal_ogrsfdriverregistrar_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrsfdriverregistrar_ce->create_object = ogrsfdriverregistrar_create_handler;
  gdal_ogrsfdriverregistrar_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;
  memcpy(&registrar_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  registrar_object_handlers.clone_obj = NULL;
}
