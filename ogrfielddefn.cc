//
// ogrfielddefn.cc
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
#include "ogrfielddefn.h"

zend_class_entry *gdal_ogrfielddefn_ce;
zend_object_handlers ogrfielddefn_object_handlers;

//
// PHP stuff
//

void ogrfielddefn_free_storage(void *object TSRMLS_DC)
{
  php_ogrfielddefn_object *obj = (php_ogrfielddefn_object *)object;
  //delete obj->fielddefn; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrfielddefn_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrfielddefn_object *obj =
    (php_ogrfielddefn_object *)emalloc(sizeof(php_ogrfielddefn_object));
  memset(obj, 0, sizeof(php_ogrfielddefn_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrfielddefn_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrfielddefn_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRFieldDefn, SetName)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;

  fielddefn->SetName(name);
}


PHP_METHOD(OGRFieldDefn, GetNameRef)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;

  const char *name = fielddefn->GetNameRef();
  if (name) {
    RETURN_STRING((char *)name, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRFieldDefn, GetType)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  RETURN_LONG((long)fielddefn->GetType());
}

PHP_METHOD(OGRFieldDefn, SetType)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;
  long type;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &type) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  fielddefn->SetType((OGRFieldType)type);
}

PHP_METHOD(OGRFieldDefn, GetJustify)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  RETURN_LONG((long)fielddefn->GetJustify());
}

PHP_METHOD(OGRFieldDefn, SetJustify)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;
  long justification;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &justification) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  fielddefn->SetJustify((OGRJustification)justification);
}

PHP_METHOD(OGRFieldDefn, GetWidth)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  RETURN_LONG(fielddefn->GetWidth());
}

PHP_METHOD(OGRFieldDefn, SetWidth)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;
  long width;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &width) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  fielddefn->SetWidth(width);
}

PHP_METHOD(OGRFieldDefn, GetPrecision)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  RETURN_LONG(fielddefn->GetPrecision());
}

PHP_METHOD(OGRFieldDefn, SetPrecision)
{
  OGRFieldDefn *fielddefn;
  php_ogrfielddefn_object *obj;
  long precision;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &precision) == FAILURE) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrfielddefn_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  fielddefn = obj->fielddefn;
  fielddefn->SetPrecision(precision);
}


//
// PHP stuff
//

zend_function_entry ogrfielddefn_methods[] = {
  PHP_ME(OGRFieldDefn, SetName,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, GetNameRef,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, GetType,                 NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, SetType,                 NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFieldDefn, GetFieldTypeName,        NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, GetJustify,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, SetJustify,              NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, GetWidth,                NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, SetWidth,                NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, GetPrecision,            NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRFieldDefn, SetPrecision,            NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFieldDefn, Set,                     NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFieldDefn, SetDefault,              NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRFieldDefn, GetDefaultRef,           NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrfielddefn_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRFieldDefn", ogrfielddefn_methods);
  gdal_ogrfielddefn_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrfielddefn_ce->create_object = ogrfielddefn_create_handler;
  memcpy(&ogrfielddefn_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrfielddefn_object_handlers.clone_obj = NULL;
}
