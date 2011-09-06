//
// ogrcoordtransform.cc
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
#include <ogr_spatialref.h>
#include "ogrcoordtransform.h"
#include "ogrspatialreference.h"

/**
 * NOTES:
 *      In PHP, the constructor is used instead of
 *      OGRCreateCoordinateTransformation()
 */

zend_class_entry *gdal_ogrcoordtransform_ce;
zend_object_handlers ogrcoordtransform_object_handlers;

//
// PHP stuff
//

void ogrcoordtransform_free_storage(void *object TSRMLS_DC)
{
  php_ogrcoordtransform_object *obj = (php_ogrcoordtransform_object *)object;
  obj->coordtransform->DestroyCT(obj->coordtransform);
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrcoordtransform_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrcoordtransform_object *obj =
    (php_ogrcoordtransform_object *)emalloc(sizeof(php_ogrcoordtransform_object));
  memset(obj, 0, sizeof(php_ogrcoordtransform_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrcoordtransform_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrcoordtransform_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRCoordinateTransformation, __construct)
{
  OGRCoordinateTransformation *coordtransform;
  php_ogrcoordtransform_object *obj;
  zval *poSource_p;
  zval *poTarget_p;
  OGRSpatialReference *poSource;
  OGRSpatialReference *poTarget;
  php_ogrspatialreference_object *poSource_obj;
  php_ogrspatialreference_object *poTarget_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"OO",
                            &poSource_p, gdal_ogrspatialreference_ce,
                            &poTarget_p, gdal_ogrspatialreference_ce)
      == FAILURE) {
    return;
  }

  poSource_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(poSource_p);
  poSource = poSource_obj->spatialreference;
  poTarget_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(poTarget_p);
  poTarget = poTarget_obj->spatialreference;

  coordtransform = OGRCreateCoordinateTransformation(poSource, poTarget);
  if (coordtransform) {
    obj = (php_ogrcoordtransform_object *)
      zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->coordtransform = coordtransform;
  } else {
    php_error_docref(NULL TSRMLS_CC, E_ERROR,
                     "OGRCreateCoordinateTransformation() returned NULL");
  }
}

PHP_METHOD(OGRCoordinateTransformation, GetSourceCS)
{
  OGRCoordinateTransformation *coordtransform;
  OGRSpatialReference *spatialref;
  php_ogrcoordtransform_object *obj;
  php_ogrspatialreference_object *spatialref_obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrcoordtransform_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  coordtransform = obj->coordtransform;

  spatialref = coordtransform->GetSourceCS();
  if (object_init_ex(return_value, gdal_ogrspatialreference_ce) != SUCCESS) {
    delete spatialref;
    RETURN_NULL();
  }
  spatialref_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  spatialref_obj->spatialreference = spatialref;
}

PHP_METHOD(OGRCoordinateTransformation, GetTargetCS)
{
  OGRCoordinateTransformation *coordtransform;
  OGRSpatialReference *spatialref;
  php_ogrcoordtransform_object *obj;
  php_ogrspatialreference_object *spatialref_obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrcoordtransform_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  coordtransform = obj->coordtransform;

  spatialref = coordtransform->GetTargetCS();
  if (object_init_ex(return_value, gdal_ogrspatialreference_ce) != SUCCESS) {
    delete spatialref;
    RETURN_NULL();
  }
  spatialref_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  spatialref_obj->spatialreference = spatialref;
}

static void doTransform(INTERNAL_FUNCTION_PARAMETERS, int ex)
{
  OGRCoordinateTransformation *coordtransform;
  OGRSpatialReference *spatialref;
  php_ogrcoordtransform_object *obj;
  php_ogrspatialreference_object *spatialref_obj;
  zval **px;
  zval **py;
  zval **pz = NULL;
  zval **data;
  zval temp;
  int count;
  HashTable *hash;
  HashPosition pos;
  double *dx;
  double *dy;
  double *dz = NULL;
  int i;
  int ret;
  int *succ;

  // http://www.phpbuilder.com/manual/zend.arguments.php
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"ZZ|Z",
                            &px, &py, &pz) == FAILURE) {
    return;
  }

  obj = (php_ogrcoordtransform_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  coordtransform = obj->coordtransform;

  if (!PZVAL_IS_REF(*px) || !PZVAL_IS_REF(*py) ||
      (pz && !PZVAL_IS_REF(*pz))) {
    php_error_docref(NULL TSRMLS_CC, E_ERROR,
                     "Arguments of OGRCoordinateTransformation::Transform"
                     " must be passed by reference");
    return;
  }
  // get float arrays
  convert_to_array(*px); // TODO: make a copy
  convert_to_array(*py);
  if (pz) {
    convert_to_array(*pz);
  }
  hash = Z_ARRVAL_P(*px);
  count = zend_hash_num_elements(hash);
  if (!count) {
    php_error_docref(NULL TSRMLS_CC, E_ERROR,
                     "Array arguments must not be empty");
    RETURN_NULL();
  }
  dx = (double*) emalloc(sizeof(*dx) * count);
  for (zend_hash_internal_pointer_reset_ex(hash, &pos), i = 0;
       zend_hash_get_current_data_ex(hash, (void**) &data, &pos) == SUCCESS;
       zend_hash_move_forward_ex(hash, &pos), ++i) {
    temp = **data;
    zval_copy_ctor(&temp);
    convert_to_double(&temp);
    dx[i] = Z_DVAL_P(&temp);
    zval_dtor(&temp);
  }
  hash = Z_ARRVAL_P(*py);
  if (zend_hash_num_elements(hash) != count) {
    php_error_docref(NULL TSRMLS_CC, E_ERROR,
                     "Array arguments must have the same length");
    efree(dx);
    RETURN_NULL();
  }
  dy = (double*) emalloc(sizeof(*dy) * count);
  for (zend_hash_internal_pointer_reset_ex(hash, &pos), i = 0;
       zend_hash_get_current_data_ex(hash, (void**) &data, &pos) == SUCCESS;
       zend_hash_move_forward_ex(hash, &pos), ++i) {
    temp = **data;
    zval_copy_ctor(&temp);
    convert_to_double(&temp);
    dy[i] = Z_DVAL_P(&temp);
    zval_dtor(&temp);
  }
  if (pz) {
    hash = Z_ARRVAL_P(*pz);
    if (zend_hash_num_elements(hash) != count) {
      php_error_docref(NULL TSRMLS_CC, E_ERROR,
                       "Array arguments must have the same length");
      efree(dx);
      efree(dy);
      RETURN_NULL();
    }
    dz = (double*) emalloc(sizeof(*dz) * count);
    for (zend_hash_internal_pointer_reset_ex(hash, &pos), i = 0;
         zend_hash_get_current_data_ex(hash, (void**) &data, &pos) == SUCCESS;
         zend_hash_move_forward_ex(hash, &pos), ++i) {
      temp = **data;
      zval_copy_ctor(&temp);
      convert_to_double(&temp);
      dz[i] = Z_DVAL_P(&temp);
      zval_dtor(&temp);
    }
  }
  // perform the transformation
  if (ex) {
    succ = (int*) emalloc(sizeof(*succ) * count);
    ret = coordtransform->TransformEx(count, dx, dy, dz, succ);
    array_init(return_value);
    for (i = 0; i < count; ++i) {
      add_next_index_bool(return_value, succ[i]);
    }
    efree(succ);
  } else {
    RETVAL_BOOL(coordtransform->Transform(count, dx, dy, dz));
  }
  // modify array args
  zval_dtor(*px);
  array_init(*px);
  zval_dtor(*py);
  array_init(*py);
  if (pz) {
    zval_dtor(*pz);
    array_init(*pz);
  }
  for (i = 0; i < count; ++i) {
    add_next_index_double(*px, dx[i]);
    add_next_index_double(*py, dy[i]);
    // char *msg;
    // asprintf(&msg, "OKA, adding [%d]: %f %f", i, dx[i], dy[i]);
    // php_log_err(msg);
    // free(msg);
    if (pz) {
      add_next_index_double(*pz, dz[i]);
    }
  }
  // clean
  efree(dx);
  efree(dy);
  if (dz) {
    efree(dz);
  }
}

PHP_METHOD(OGRCoordinateTransformation, Transform)
{
  doTransform(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

PHP_METHOD(OGRCoordinateTransformation, TransformEx)
{
  doTransform(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}


//
// PHP stuff
//

ZEND_BEGIN_ARG_INFO(arginfo_transform_methods, 1)
ZEND_END_ARG_INFO();

function_entry ogrcoordtransform_methods[] = {
  PHP_ME(OGRCoordinateTransformation, __construct, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRCoordinateTransformation, GetSourceCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRCoordinateTransformation, GetTargetCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRCoordinateTransformation, Transform,
         arginfo_transform_methods, ZEND_ACC_PUBLIC)
  PHP_ME(OGRCoordinateTransformation, TransformEx,
         arginfo_transform_methods, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrcoordtransform_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRCoordinateTransformation",
                   ogrcoordtransform_methods);
  gdal_ogrcoordtransform_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrcoordtransform_ce->create_object = ogrcoordtransform_create_handler;
  memcpy(&ogrcoordtransform_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrcoordtransform_object_handlers.clone_obj = NULL;
}
