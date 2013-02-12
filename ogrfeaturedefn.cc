//
// ogrfeaturedefn.cc
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
#include "php_gdal.h"
#include <ogr_feature.h>
#include "ogrfeaturedefn.h"
#include "ogrfielddefn.h"

zend_class_entry *gdal_ogrfeaturedefn_ce;
zend_object_handlers ogrfeaturedefn_object_handlers;

//
// PHP stuff
//

void ogrfeaturedefn_free_storage(void *object TSRMLS_DC)
{
  php_ogrfeaturedefn_object *obj = (php_ogrfeaturedefn_object *)object;
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrfeaturedefn_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrfeaturedefn_object *obj =
    (php_ogrfeaturedefn_object *)emalloc(sizeof(php_ogrfeaturedefn_object));
  memset(obj, 0, sizeof(php_ogrfeaturedefn_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrfeaturedefn_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrfeaturedefn_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRFeatureDefn, GetName)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;

  const char *name = featuredefn->GetName();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRFeatureDefn, GetFieldCount)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG(featuredefn->GetFieldCount());
}

PHP_METHOD(OGRFeatureDefn, GetFieldDefn)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *fielddefn_obj;
  long index;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &index) == FAILURE) {
    return;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;

  fielddefn = featuredefn->GetFieldDefn(index);
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

PHP_METHOD(OGRFeatureDefn, GetFieldIndex)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG(featuredefn->GetFieldIndex(name));
}

PHP_METHOD(OGRFeatureDefn, AddFieldDefn)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;
  zval *fielddefnp;
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *fielddefn_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &fielddefnp, gdal_ogrfielddefn_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;

  fielddefn_obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(fielddefnp);
  fielddefn = fielddefn_obj->fielddefn;
  featuredefn->AddFieldDefn(fielddefn);
}

PHP_METHOD(OGRFeatureDefn, GetGeomType)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG((long)featuredefn->GetGeomType());
}

PHP_METHOD(OGRFeatureDefn, SetGeomType)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;
  long geomType;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &geomType) == FAILURE) {
    return;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  featuredefn->SetGeomType((OGRwkbGeometryType)geomType);
}

PHP_METHOD(OGRFeatureDefn, Clone)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;
  OGRFeatureDefn *clone_featuredefn;
  php_ogrfeaturedefn_object *clone_obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;

  clone_featuredefn = featuredefn->Clone();
  if (!clone_featuredefn) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_ogrfeaturedefn_ce) != SUCCESS) {
    RETURN_NULL();
  }
  clone_obj = (php_ogrfeaturedefn_object*)
    zend_objects_get_address(return_value TSRMLS_CC);
  clone_obj->featuredefn = clone_featuredefn;
}

PHP_METHOD(OGRFeatureDefn, Reference)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG(featuredefn->Reference());
}

PHP_METHOD(OGRFeatureDefn, Dereference)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG(featuredefn->Dereference());
}

PHP_METHOD(OGRFeatureDefn, GetReferenceCount)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  RETURN_LONG(featuredefn->GetReferenceCount());
}

PHP_METHOD(OGRFeatureDefn, Release)
{
  OGRFeatureDefn *featuredefn;
  php_ogrfeaturedefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfeaturedefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  featuredefn = obj->featuredefn;
  featuredefn->Release();
}


//
// PHP stuff
//

zend_function_entry ogrfeaturedefn_methods[] = {
  PHP_ME(OGRFeatureDefn, GetName,               NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, GetFieldCount,         NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, GetFieldDefn,          NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, GetFieldIndex,         NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, AddFieldDefn,          NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, GetGeomType,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, SetGeomType,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, Clone,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, Reference,             NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, Dereference,           NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, GetReferenceCount,     NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFeatureDefn, Release,               NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFeatureDefn, CreateFeatureDefn,
  //        NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  // PHP_ME(OGRFeatureDefn, DestroyFeatureDefn,
  //        NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrfeaturedefn_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRFeatureDefn", ogrfeaturedefn_methods);
  gdal_ogrfeaturedefn_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrfeaturedefn_ce->create_object = ogrfeaturedefn_create_handler;
  memcpy(&ogrfeaturedefn_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrfeaturedefn_object_handlers.clone_obj = NULL;
}
