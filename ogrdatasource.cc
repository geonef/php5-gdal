

#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>
#include "php.h"
#include "php_gdal.h"
#include "ogrdatasource.h"
#include "ogrsfdriver.h"
#include "ogrlayer.h"
#include "ogrspatialreference.h"

zend_class_entry *gdal_ogrdatasource_ce;
zend_object_handlers ogrdatasource_object_handlers;

void php_gdal_ogrdatasource_add_to_hash(php_ogrdatasource_object *obj)
{
  ////
  char *msg;
  HashPosition pos;

  // asprintf(&msg, "php5-gdal: ogrdatasource_add_to_hash");
  // php_log_err(msg);
  // free(msg);
  ////
  zend_hash_next_index_insert(&GDAL_G(ogrDataSources),
                              &(obj->datasource),
                              sizeof(obj->datasource), NULL);
  zend_hash_internal_pointer_end_ex(&GDAL_G(ogrDataSources), &pos);
  if (zend_hash_get_current_key_ex(&GDAL_G(ogrDataSources), NULL, NULL,
                                   &obj->hashIndex, 0, &pos)
      != HASH_KEY_IS_LONG) {
    php_log_err("php5-gdal: failed to get hash index for datasource");
  }
  ////
  asprintf(&msg, "php5-gdal: got OGR datasource hash index: %d",
           (int)obj->hashIndex);
  php_log_err(msg);
  free(msg);
  ////
  //zend_hash_num_elements
}

void php_gdal_ogrdatasource_ptr_dtor(void **ptr)
{
  OGRDataSource *datasource = (OGRDataSource *)*ptr;
  ////
  // char *msg;
  // asprintf(&msg, "php5-gdal: php_gdal_ogrdatasource_ptr_dtor ############");
  // php_log_err(msg);
  // free(msg);
  ////
  OGRDataSource::DestroyDataSource(datasource);
}

//
// PHP stuff
//

void ogrdatasource_free_storage(void *object TSRMLS_DC)
{
  char *msg;
  int i, i2;
  php_ogrdatasource_object *obj = (php_ogrdatasource_object *)object;

  // i = obj->datasource->GetRefCount();
  // obj->datasource->Dereference();
  // //obj->datasource->Release(); // Release() is for "shared" (?) datasources
  // i2 = obj->datasource->GetRefCount();
  // asprintf(&msg, "OGRDataSource FREE; count_bef=%d, count_aft=%d", i, i2);
  // php_log_err(msg);
  // free(msg);
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrdatasource_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrdatasource_object *obj =
    (php_ogrdatasource_object *)emalloc(sizeof(php_ogrdatasource_object));
  memset(obj, 0, sizeof(php_ogrdatasource_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref,
                 (void *)&tmp, sizeof(zval *));

  retval.handle =
    zend_objects_store_put(obj, NULL,
                           ogrdatasource_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrdatasource_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRDataSource, GetName)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  const char *name = datasource->GetName();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRDataSource, GetLayerCount)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  RETURN_LONG(datasource->GetLayerCount());
}

PHP_METHOD(OGRDataSource, GetLayer)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  OGRLayer *layer;
  php_ogrlayer_object *layer_obj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &index) == FAILURE) {
    return;
  }
  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;

  int i = datasource->GetRefCount();
  layer = datasource->GetLayer(index);
  if (!layer) {
    RETURN_NULL();
  }
  ////
  char *msg; int i2;
  i2 = datasource->GetRefCount();
  asprintf(&msg, "OGRDataSource::GetLayer bef=%d aft=%d", i, i2);
  php_log_err(msg);
  free(msg);
  i = layer->GetRefCount();
  asprintf(&msg, "OGRDataSource::GetLayer layerrefC=%d", i);
  php_log_err(msg);
  free(msg);
  ////
  if (object_init_ex(return_value, gdal_ogrlayer_ce) != SUCCESS) {
    RETURN_NULL();
  }
  layer_obj = (php_ogrlayer_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  layer_obj->layer = layer;
}

PHP_METHOD(OGRDataSource, GetLayerByName)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  OGRLayer *layer;
  php_ogrlayer_object *layer_obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }
  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;

  int i = datasource->GetRefCount();
  layer = datasource->GetLayerByName(name);
  if (!layer) {
    RETURN_NULL();
  }
  ////
  char *msg; int i2;
  i2 = datasource->GetRefCount();
  asprintf(&msg, "OGRDataSource::GetLayerByName bef=%d aft=%d", i, i2);
  php_log_err(msg);
  free(msg);
  i = layer->GetRefCount();
  asprintf(&msg, "OGRDataSource::GetLayer layerrefC=%d", i);
  php_log_err(msg);
  free(msg);
  ////
  if (object_init_ex(return_value, gdal_ogrlayer_ce) != SUCCESS) {
    RETURN_NULL();
  }
  layer_obj = (php_ogrlayer_object*)
    zend_object_store_get_object
    //zend_objects_get_address
    (return_value TSRMLS_CC);
  layer_obj->layer = layer;
}

PHP_METHOD(OGRDataSource, DeleteLayer)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &index) == FAILURE) {
    return;
  }
  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;

  RETURN_LONG(datasource->DeleteLayer(index));
}

PHP_METHOD(OGRDataSource, TestCapability)
{
  // Available capabilities as of 1.7.3 (ogr_core.h):
  //    "CreateLayer", "DeleteLayer"
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  char *strcapability = NULL;
  int strcapability_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &strcapability, &strcapability_len) == FAILURE) {
    return;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  RETURN_BOOL(datasource->TestCapability(strcapability));
}

PHP_METHOD(OGRDataSource, CreateLayer)
{
  // Available capabilities as of 1.7.3 (ogr_core.h):
  //    "CreateLayer", "DeleteLayer"
  char *name = NULL;
  int name_len;
  OGRwkbGeometryType gtype = wkbUnknown;
  zval *spatialrefp = NULL;
  php_ogrspatialreference_object *spatialref_obj;
  OGRSpatialReference *spatialref = NULL;
  php_ogrdatasource_object *obj;
  OGRDataSource *datasource;
  OGRLayer *layer;
  php_ogrlayer_object *layer_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s|O!l",
                            &name, &name_len, &spatialrefp,
                            gdal_ogrspatialreference_ce, &gtype) == FAILURE) {
    return;
  }

  if (spatialrefp) {
    spatialref_obj = (php_ogrspatialreference_object *)
      zend_object_store_get_object(spatialrefp);
    spatialref = spatialref_obj->spatialreference;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;

  layer = datasource->CreateLayer(name, spatialref, gtype, NULL /* TODO: pass options */);
  if (!layer) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrlayer_ce) != SUCCESS) {
    RETURN_NULL();
  }
  layer_obj = (php_ogrlayer_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  layer_obj->layer = layer;

}

PHP_METHOD(OGRDataSource, SyncToDisk)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  OGRErr error;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  error = datasource->SyncToDisk();
  RETURN_LONG(error);
}

PHP_METHOD(OGRDataSource, GetDriver)
{
  OGRDataSource *datasource;
  OGRSFDriver *driver;
  php_ogrdatasource_object *obj;
  driver_object *driver_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  driver = datasource->GetDriver();
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrsfdriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driver_obj = (driver_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  driver_obj->driver = driver;
}

PHP_METHOD(OGRDataSource, Reference)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  RETURN_LONG(datasource->Reference());
}

PHP_METHOD(OGRDataSource, Dereference)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  RETURN_LONG(datasource->Dereference());
}

PHP_METHOD(OGRDataSource, GetRefCount)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;
  RETURN_LONG(datasource->GetRefCount());
}

PHP_METHOD(OGRDataSource, Close)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  zval *p;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrdatasource_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  datasource = obj->datasource;

  ////
  char *msg;
  asprintf(&msg, "OGRDataSource::DestroyDataSource %x", datasource);
  php_log_err(msg);
  free(msg);
  ////
  //OGRDataSource::DestroyDataSource(datasource);
}

PHP_METHOD(OGRDataSource, DestroyDataSource)
{
  OGRDataSource *datasource;
  php_ogrdatasource_object *obj;
  zval *p;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &p, gdal_ogrdatasource_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrdatasource_object *)zend_object_store_get_object(p);
  if (obj) {
    datasource = obj->datasource;
    OGRDataSource::DestroyDataSource(datasource);
  }
}


//
// PHP stuff
//

function_entry ogrdatasource_methods[] = {
  PHP_ME(OGRDataSource, GetName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, GetLayerCount, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, GetLayer, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, GetLayerByName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, DeleteLayer, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, TestCapability, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, CreateLayer, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, CopyLayer, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, GetStyleTable, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, SetStyleTableDirectly, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, SetStyleTable, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, ExecuteSQL, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRDataSource, ReleaseResultSet, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, SyncToDisk, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, GetDriver, NULL, ZEND_ACC_PUBLIC)
  //PHP_ME(OGRDataSource, SetDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, Reference, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, Dereference, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, GetRefCount, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRDataSource, Close, NULL, ZEND_ACC_PUBLIC) // extra
  PHP_ME(OGRDataSource, DestroyDataSource, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrdatasource_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRDataSource", ogrdatasource_methods);
  gdal_ogrdatasource_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrdatasource_ce->create_object = ogrdatasource_create_handler;
  memcpy(&ogrdatasource_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrdatasource_object_handlers.clone_obj = NULL;
}
