//
// ogrenvelope.cc
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

#include "php.h"
#include "php_gdal.h"
#include <ogr_core.h>
#include "ogrenvelope.h"

zend_class_entry *gdal_ogrenvelope_ce;
zend_object_handlers ogrenvelope_object_handlers;

//
// PHP stuff
//

void ogrenvelope_free_storage(void *object TSRMLS_DC)
{
  php_ogrenvelope_object *obj = (php_ogrenvelope_object *)object;
  //delete obj->envelope; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrenvelope_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrenvelope_object *obj =
    (php_ogrenvelope_object *)emalloc(sizeof(php_ogrenvelope_object));
  memset(obj, 0, sizeof(php_ogrenvelope_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrenvelope_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrenvelope_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGREnvelope, IsInit)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;
  RETURN_BOOL(envelope->IsInit());
}

PHP_METHOD(OGREnvelope, Merge)
{
  OGREnvelope *envelope;
  OGREnvelope *arg_env;
  php_ogrenvelope_object *obj;
  OGRFieldDefn *fielddefn;
  OGRErr error;
  zval *arg_envp;
  php_ogrenvelope_object *arg_env_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &arg_envp, gdal_ogrenvelope_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  arg_env_obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(arg_envp);
  arg_env = arg_env_obj->envelope;

  envelope->Merge(*arg_env);
}

PHP_METHOD(OGREnvelope, MergeCoords)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;
  double dfX, dfY;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"dd",
                            &dfX, &dfY) == FAILURE) {
    return;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  envelope->Merge(dfX, dfY);
}

PHP_METHOD(OGREnvelope, Intersect)
{
  OGREnvelope *envelope;
  OGREnvelope *arg_env;
  php_ogrenvelope_object *obj;
  OGRFieldDefn *fielddefn;
  OGRErr error;
  zval *arg_envp;
  php_ogrenvelope_object *arg_env_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &arg_envp, gdal_ogrenvelope_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  arg_env_obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(arg_envp);
  arg_env = arg_env_obj->envelope;

  envelope->Intersect(*arg_env);
}

PHP_METHOD(OGREnvelope, Intersects)
{
  OGREnvelope *envelope;
  OGREnvelope *arg_env;
  php_ogrenvelope_object *obj;
  OGRFieldDefn *fielddefn;
  OGRErr error;
  zval *arg_envp;
  php_ogrenvelope_object *arg_env_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &arg_envp, gdal_ogrenvelope_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  arg_env_obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(arg_envp);
  arg_env = arg_env_obj->envelope;

  RETURN_BOOL(envelope->Intersects(*arg_env));
}

PHP_METHOD(OGREnvelope, Contains)
{
  OGREnvelope *envelope;
  OGREnvelope *arg_env;
  php_ogrenvelope_object *obj;
  OGRFieldDefn *fielddefn;
  OGRErr error;
  zval *arg_envp;
  php_ogrenvelope_object *arg_env_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &arg_envp, gdal_ogrenvelope_ce) == FAILURE) {
    return;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  arg_env_obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(arg_envp);
  arg_env = arg_env_obj->envelope;

  RETURN_BOOL(envelope->Contains(*arg_env));
}

PHP_METHOD(OGREnvelope, GetMinX)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;
  RETURN_DOUBLE(envelope->MinX);
}

PHP_METHOD(OGREnvelope, GetMinY)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;
  RETURN_DOUBLE(envelope->MinY);
}

PHP_METHOD(OGREnvelope, GetMaxX)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;
  RETURN_DOUBLE(envelope->MaxX);
}

PHP_METHOD(OGREnvelope, GetMaxY)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;
  RETURN_DOUBLE(envelope->MaxY);
}

PHP_METHOD(OGREnvelope, AsArray)
{
  OGREnvelope *envelope;
  php_ogrenvelope_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrenvelope_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  envelope = obj->envelope;

  array_init(return_value);
  add_next_index_double(return_value, envelope->MinX);
  add_next_index_double(return_value, envelope->MinY);
  add_next_index_double(return_value, envelope->MaxX);
  add_next_index_double(return_value, envelope->MaxY);
}


//
// PHP stuff
//

function_entry ogrenvelope_methods[] = {
  PHP_ME(OGREnvelope, IsInit, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, Merge, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, MergeCoords, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, Intersect, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, Intersects, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, Contains, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, GetMinX, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, GetMinY, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, GetMaxX, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGREnvelope, GetMaxY, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrenvelope_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGREnvelope", ogrenvelope_methods);
  gdal_ogrenvelope_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrenvelope_ce->create_object = ogrenvelope_create_handler;
  memcpy(&ogrenvelope_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrenvelope_object_handlers.clone_obj = NULL;
}
