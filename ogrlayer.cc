

#include "php.h"
#include "php_gdal.h"
#include <ogrsf_frmts.h>
#include "ogrlayer.h"
#include "ogrfeaturedefn.h"
#include "ogrfielddefn.h"
#include "ogrenvelope.h"
#include "ogrspatialreference.h"
#include "ogrfeature.h"

zend_class_entry *gdal_ogrlayer_ce;
zend_object_handlers ogrlayer_object_handlers;

//
// PHP stuff
//

void ogrlayer_free_storage(void *object TSRMLS_DC)
{
  php_ogrlayer_object *obj = (php_ogrlayer_object *)object;
  //delete obj->layer; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrlayer_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrlayer_object *obj =
    (php_ogrlayer_object *)emalloc(sizeof(php_ogrlayer_object));
  memset(obj, 0, sizeof(php_ogrlayer_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrlayer_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrlayer_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRLayer, SetSpatialFilterRect)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  double min_x, min_y, max_x, max_y;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"dddd",
                            &min_x, &min_y, &max_x, &max_y) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  layer->SetSpatialFilterRect(min_x, min_y, max_x, max_y);
}

PHP_METHOD(OGRLayer, SetAttributeFilter)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  char *filter;
  int filter_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &filter, &filter_len) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  RETURN_LONG(layer->SetAttributeFilter(filter));
}

PHP_METHOD(OGRLayer, ResetReading)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  layer->ResetReading();
}

PHP_METHOD(OGRLayer, GetNextFeature)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFeature *feature;
  php_ogrfeature_object *feature_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  feature = layer->GetNextFeature();

  if (!feature) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfeature_ce) != SUCCESS) {
    OGRFeature::DestroyFeature(feature);
    RETURN_NULL();
  }
  feature_obj = (php_ogrfeature_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  feature_obj->feature = feature;
}

PHP_METHOD(OGRLayer, SetNextByIndex)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &index) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->SetNextByIndex(index));
}

PHP_METHOD(OGRLayer, GetFeature)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFeature *feature;
  php_ogrfeature_object *feature_obj;
  long fid;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &fid) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  feature = layer->GetFeature(fid);

  if (!feature) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfeature_ce) != SUCCESS) {
    OGRFeature::DestroyFeature(feature);
    RETURN_NULL();
  }
  feature_obj = (php_ogrfeature_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  feature_obj->feature = feature;
}

PHP_METHOD(OGRLayer, SetFeature)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFeature *feature;
  zval *featurep;
  php_ogrfeature_object *feature_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &featurep, gdal_ogrfeature_ce) == FAILURE) {
    return;
  }

  feature_obj = (php_ogrfeature_object *)
    zend_object_store_get_object(featurep);
  feature = feature_obj->feature;

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  RETURN_LONG(layer->SetFeature(feature));
}

PHP_METHOD(OGRLayer, CreateFeature)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFeature *feature;
  zval *featurep;
  php_ogrfeature_object *feature_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &featurep, gdal_ogrfeature_ce) == FAILURE) {
    return;
  }

  feature_obj = (php_ogrfeature_object *)
    zend_object_store_get_object(featurep);
  feature = feature_obj->feature;

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  RETURN_LONG(layer->CreateFeature(feature));
}

PHP_METHOD(OGRLayer, DeleteFeature)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  long fid;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &fid) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->DeleteFeature(fid));
}

PHP_METHOD(OGRLayer, GetLayerDefn)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *defn_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  featuredefn = layer->GetLayerDefn();
  if (!featuredefn) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfeaturedefn_ce) != SUCCESS) {
    RETURN_NULL();
  }
  defn_obj = (php_ogrfeaturedefn_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  defn_obj->featuredefn = featuredefn;
}

PHP_METHOD(OGRLayer, GetSpatialRef)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *spatialreference_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  spatialreference = layer->GetSpatialRef();
  if (!spatialreference) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrspatialreference_ce) != SUCCESS) {
    RETURN_NULL();
  }
  spatialreference_obj = (php_ogrspatialreference_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  spatialreference_obj->spatialreference = spatialreference;
}

PHP_METHOD(OGRLayer, GetFeatureCount)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  long index;
  zend_bool force = 1;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"|b",
                            &force) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->GetFeatureCount(force));
}

PHP_METHOD(OGRLayer, GetExtent)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGREnvelope *envelope;
  php_ogrenvelope_object *envelope_obj;
  OGRErr error;
  zend_bool bforce;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"b",
                            &bforce) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  envelope = new OGREnvelope();
  error = layer->GetExtent(envelope, bforce);
  if (error != OGRERR_NONE) {
    delete envelope;
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrenvelope_ce) != SUCCESS) {
    delete envelope;
    RETURN_NULL();
  }
  envelope_obj = (php_ogrenvelope_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  envelope_obj->envelope = envelope;
}

PHP_METHOD(OGRLayer, TestCapability)
{
  // Available capabilities as of 1.7.3 (ogr_core.h):
  //    "RandomRead"            "SequentialWrite"       "RandomWrite"
  //    "FastSpatialFilter"     "FastFeatureCount"      "FastGetExtent"
  //    "CreateField"           "Transactions"          "DeleteFeature"
  //    "FastSetNextByIndex"    "StringsAsUTF8"
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  char *strcapability = NULL;
  int strcapability_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &strcapability, &strcapability_len) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  RETURN_BOOL(layer->TestCapability(strcapability));
}

PHP_METHOD(OGRLayer, GetInfo)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRErr error;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  const char *value = layer->GetInfo(name);
  if (name) {
    RETURN_STRING((char *)value, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRLayer, CreateField)
{
  // TODO: check object type
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRFieldDefn *fielddefn;
  OGRErr error;
  zval *fielddefnp;
  php_ogrfielddefn_object *fielddefn_obj;
  zend_bool approx_ok = 1;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O|b",
                            &fielddefnp, gdal_ogrfielddefn_ce,
                            &approx_ok) == FAILURE) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;

  fielddefn_obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(fielddefnp);
  fielddefn = fielddefn_obj->fielddefn;
  error = layer->CreateField(fielddefn, approx_ok);
  RETURN_LONG(error);
}

PHP_METHOD(OGRLayer, SyncToDisk)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRErr error;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  error = layer->SyncToDisk();
  RETURN_LONG(error);
}

PHP_METHOD(OGRLayer, StartTransaction)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRErr error;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  error = layer->StartTransaction();
  RETURN_LONG(error);
}

PHP_METHOD(OGRLayer, CommitTransaction)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRErr error;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  error = layer->CommitTransaction();
  RETURN_LONG(error);
}

PHP_METHOD(OGRLayer, RollbackTransaction)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;
  OGRErr error;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  error = layer->RollbackTransaction();
  RETURN_LONG(error);
}

PHP_METHOD(OGRLayer, GetFIDColumn)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  const char *name = layer->GetFIDColumn();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRLayer, GetGeometryColumn)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  const char *name = layer->GetGeometryColumn();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRLayer, Reference)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->Reference());
}

PHP_METHOD(OGRLayer, Dereference)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->Dereference());
}

PHP_METHOD(OGRLayer, GetRefCount)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->GetRefCount());
}

PHP_METHOD(OGRLayer, GetFeaturesRead)
{
  OGRLayer *layer;
  php_ogrlayer_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrlayer_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  layer = obj->layer;
  RETURN_LONG(layer->GetFeaturesRead());
}



//
// PHP stuff
//

function_entry ogrlayer_methods[] = {
  // PHP_ME(OGRLayer, GetSpatialFilter,            NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRLayer, SetSpatialFilter,            NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, SetSpatialFilterRect,        NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, SetAttributeFilter,          NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, ResetReading,                NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetNextFeature,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, SetNextByIndex,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetFeature,                  NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, SetFeature,                  NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, CreateFeature,               NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, DeleteFeature,               NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetLayerDefn,                NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetSpatialRef,               NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetFeatureCount,             NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetExtent,                   NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, TestCapability,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetInfo,                     NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, CreateField,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, SyncToDisk,                  NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRLayer, GetStyleTable,               NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRLayer, SetStyleTableDirectly,       NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRLayer, SetStyleTable,               NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, StartTransaction,            NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, CommitTransaction,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, RollbackTransaction,         NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetFIDColumn,                NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetGeometryColumn,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, Reference,                   NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, Dereference,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetRefCount,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRLayer, GetFeaturesRead,             NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrlayer_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRLayer", ogrlayer_methods);
  gdal_ogrlayer_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrlayer_ce->create_object = ogrlayer_create_handler;
  memcpy(&ogrlayer_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrlayer_object_handlers.clone_obj = NULL;
}
