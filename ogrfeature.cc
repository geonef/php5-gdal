//
// ogrfeature.cc
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
#include "php_gdal.h"
#include <ogr_feature.h>
#include "ogrfeature.h"
#include "ogrfeaturedefn.h"
#include "ogrfielddefn.h"

zend_class_entry *gdal_ogrfeature_ce;
zend_object_handlers ogrfeature_object_handlers;

//
// PHP stuff
//

void ogrfeature_free_storage(void *object TSRMLS_DC)
{
  php_ogrfeature_object *obj = (php_ogrfeature_object *)object;
  // TODO: check ownership
  // OGRFeature::DestroyFeature(obj->feature);
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrfeature_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrfeature_object *obj =
    (php_ogrfeature_object *)emalloc(sizeof(php_ogrfeature_object));
  memset(obj, 0, sizeof(php_ogrfeature_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrfeature_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrfeature_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRFeature, GetDefnRef)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *featuredefn_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  featuredefn = feature->GetDefnRef();
  if (!featuredefn) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfeaturedefn_ce) != SUCCESS) {
    RETURN_NULL();
  }
  featuredefn_obj = (php_ogrfeaturedefn_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  featuredefn_obj->featuredefn = featuredefn;
}

PHP_METHOD(OGRFeature, GetFieldCount)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;
  RETURN_LONG(feature->GetFieldCount());
}

PHP_METHOD(OGRFeature, GetFieldDefnRef)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *fielddefn_obj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &index) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  fielddefn = feature->GetFieldDefnRef(index);
  if (!fielddefn) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfielddefn_ce) != SUCCESS) {
    RETURN_NULL();
  }
  fielddefn_obj = (php_ogrfielddefn_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  fielddefn_obj->fielddefn = fielddefn;
}

PHP_METHOD(OGRFeature, GetFieldIndex)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;
  RETURN_LONG(feature->GetFieldIndex(name));
}

PHP_METHOD(OGRFeature, IsFieldSet)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  RETURN_BOOL(feature->IsFieldSet(idx));
}

PHP_METHOD(OGRFeature, UnsetField)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  feature->UnsetField(idx);
}

PHP_METHOD(OGRFeature, GetFieldAsInteger)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  RETURN_LONG(feature->GetFieldAsInteger(idx));
}

PHP_METHOD(OGRFeature, GetFieldAsDouble)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  RETURN_DOUBLE(feature->GetFieldAsInteger(idx));
}

PHP_METHOD(OGRFeature, GetFieldAsString)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  const char *str = feature->GetFieldAsString(idx);
  RETURN_STRING((char *)str, 1);
}

PHP_METHOD(OGRFeature, GetFID)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  RETURN_LONG(feature->GetFID());
}

PHP_METHOD(OGRFeature, SetFID)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  long fid;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &fid) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  feature->SetFID(fid);
}

PHP_METHOD(OGRFeature, SetFrom)
{
  zval *from_featurep;
  zend_bool forgiving = 1;
  OGRFeature *from_feature;
  php_ogrfeature_object *from_feature_obj;
  OGRFeature *feature;
  php_ogrfeature_object *obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O|b",
                            &from_featurep, gdal_ogrfeature_ce,
                            &forgiving) == FAILURE) {
    return;
  }

  from_feature_obj = (php_ogrfeature_object *)
    zend_object_store_get_object(from_featurep);
  from_feature = from_feature_obj->feature;

  obj = (php_ogrfeature_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  feature = obj->feature;

  RETURN_LONG(feature->SetFrom(from_feature, forgiving));
}

PHP_METHOD(OGRFeature, CreateFeature)
{
  zval *featuredefnp;
  php_ogrfeaturedefn_object *featuredefn_obj;
  OGRFeatureDefn *featuredefn;
  OGRFeature *feature;
  php_ogrfeature_object *feature_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &featuredefnp, gdal_ogrfeaturedefn_ce)
      == FAILURE) {
    return;
  }

  featuredefn_obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(featuredefnp);
  featuredefn = featuredefn_obj->featuredefn;

  feature = OGRFeature::CreateFeature(featuredefn);
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

PHP_METHOD(OGRFeature, DestroyFeature)
{
  OGRFeature *feature;
  php_ogrfeature_object *obj;
  zval *p;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &p, gdal_ogrfeature_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrfeature_object *)zend_object_store_get_object(p);
  if (obj) {
    feature = obj->feature;
    OGRFeature::DestroyFeature(feature);
  }
}


//
// PHP stuff
//

function_entry ogrfeature_methods[] = {
  PHP_ME(OGRFeature, GetDefnRef, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetGeometryDirectly, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetGeometry, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetGeometryRef, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, StealGeometry, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, Clone, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, Equal, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldCount, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldDefnRef, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldIndex, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, IsFieldSet, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, UnsetField, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetRawFieldRef, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldAsInteger, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldAsDouble, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, GetFieldAsString, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsIntegerList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsDoubleList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsStringList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsBinary, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsDateTime, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsInteger, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsDouble, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsString, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsIntegerList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsDoubleList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetFieldAsStringList, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetField, NULL, ZEND_ACC_PUBLIC) //
  PHP_ME(OGRFeature, GetFID, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, SetFID, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, DumpReadable, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, SetFrom, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, RemapFields, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetStyleString, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetStyleStringDirectly, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, GetStyleTable, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetStyleTable, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeature, SetStyleTableDirectly, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeature, CreateFeature, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(OGRFeature, DestroyFeature, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrfeature_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRFeature", ogrfeature_methods);
  gdal_ogrfeature_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrfeature_ce->create_object = ogrfeature_create_handler;
  memcpy(&ogrfeature_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrfeature_object_handlers.clone_obj = NULL;
}
