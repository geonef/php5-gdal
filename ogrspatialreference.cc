
#include "php.h"
#include "php_gdal.h"
#include <ogr_spatialref.h>
#include "ogrspatialreference.h"

zend_class_entry *gdal_ogrspatialreference_ce;
zend_object_handlers ogrspatialreference_object_handlers;

//
// PHP stuff
//

void ogrspatialreference_free_storage(void *object TSRMLS_DC)
{
  php_ogrspatialreference_object *obj = (php_ogrspatialreference_object *)object;
  // TODO: check ownership
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value ogrspatialreference_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrspatialreference_object *obj =
    (php_ogrspatialreference_object *)emalloc(sizeof(php_ogrspatialreference_object));
  memset(obj, 0, sizeof(php_ogrspatialreference_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrspatialreference_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrspatialreference_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(OGRSpatialReference, __construct)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *wkt = NULL;
  int wkt_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"|s",
                            &wkt, &wkt_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  obj->spatialreference = new OGRSpatialReference(wkt);
}

PHP_METHOD(OGRSpatialReference, Reference)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->Reference());
}

PHP_METHOD(OGRSpatialReference, Dereference)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->Dereference());
}

PHP_METHOD(OGRSpatialReference, GetReferenceCount)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->GetReferenceCount());
}

PHP_METHOD(OGRSpatialReference, Release)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_NULL();
}

PHP_METHOD(OGRSpatialReference, Clone)
{
  OGRSpatialReference *spatialreference;
  OGRSpatialReference *spatialreference_clone;
  php_ogrspatialreference_object *obj;
  php_ogrspatialreference_object *obj_clone;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  spatialreference_clone = spatialreference->Clone();
  if (object_init_ex(return_value, gdal_ogrspatialreference_ce) != SUCCESS) {
    spatialreference_clone->Release();
    RETURN_NULL();
  }
  obj_clone = (php_ogrspatialreference_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  obj_clone->spatialreference = spatialreference_clone;
}

PHP_METHOD(OGRSpatialReference, CloneGeogCS)
{
  OGRSpatialReference *spatialreference;
  OGRSpatialReference *spatialreference_clone;
  php_ogrspatialreference_object *obj;
  php_ogrspatialreference_object *obj_clone;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  spatialreference_clone = spatialreference->CloneGeogCS();
  if (object_init_ex(return_value, gdal_ogrspatialreference_ce) != SUCCESS) {
    spatialreference_clone->Release();
    RETURN_NULL();
  }
  obj_clone = (php_ogrspatialreference_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  obj_clone->spatialreference = spatialreference_clone;
}

PHP_METHOD(OGRSpatialReference, exportToWkt)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  char *ret;
  zval *tmp;
  int err;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->exportToWkt(&str);
  if (err == OGRERR_NONE) {
    ret = estrdup(str);
    OGRFree(str);
    RETURN_STRING(ret, 0);
  } else {
    RETURN_LONG(err);
  }
}

PHP_METHOD(OGRSpatialReference, exportToPrettyWkt)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  zend_bool bSimplify = 0;
  char *str;
  char *ret;
  zval *tmp;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"|b",
                            &bSimplify) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->exportToPrettyWkt(&str, bSimplify);
  if (err == OGRERR_NONE) {
    ret = estrdup(str);
    OGRFree(str);
    RETURN_STRING(ret, 0);
  } else {
    RETURN_LONG(err);
  }
}

PHP_METHOD(OGRSpatialReference, exportToProj4)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  char *ret;
  zval *tmp;
  int err;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->exportToProj4(&str);
  if (err == OGRERR_NONE) {
    ret = estrdup(str);
    OGRFree(str);
    RETURN_STRING(ret, 0);
  } else {
    RETURN_LONG(err);
  }
}

PHP_METHOD(OGRSpatialReference, exportToXML)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  char *ret;
  zval *tmp;
  int err;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->exportToXML(&str);
  if (err == OGRERR_NONE) {
    ret = estrdup(str);
    OGRFree(str);
    RETURN_STRING(ret, 0);
  } else {
    RETURN_LONG(err);
  }
}

PHP_METHOD(OGRSpatialReference, importFromProj4)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->importFromProj4(str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, importFromWkt)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->importFromWkt(&str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, importFromEPSG)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  long code;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &code) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->importFromEPSG(code);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, importFromEPSGA)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  long code;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &code) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->importFromEPSGA(code);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, Validate)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->Validate());
}

PHP_METHOD(OGRSpatialReference, FixupOrdering)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->FixupOrdering());
}

PHP_METHOD(OGRSpatialReference, Fixup)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_LONG(spatialreference->Fixup());
}

PHP_METHOD(OGRSpatialReference, IsGeographic)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_BOOL(spatialreference->IsGeographic());
}

PHP_METHOD(OGRSpatialReference, IsProjected)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_BOOL(spatialreference->IsProjected());
}

PHP_METHOD(OGRSpatialReference, IsLocal)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_BOOL(spatialreference->IsLocal());
}

// PHP_METHOD(OGRSpatialReference, IsVertical)
// {
//   OGRSpatialReference *spatialreference;
//   php_ogrspatialreference_object *obj;

//   if (ZEND_NUM_ARGS() != 0) {
//     WRONG_PARAM_COUNT;
//   }

//   obj = (php_ogrspatialreference_object *)
//     zend_object_store_get_object(getThis() TSRMLS_CC);
//   spatialreference = obj->spatialreference;
//   RETURN_BOOL(spatialreference->IsVertical());
// }

PHP_METHOD(OGRSpatialReference, IsSameGeogCS)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  zval *poOther_p;
  OGRSpatialReference *poOther;
  php_ogrspatialreference_object *poOther_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                           &poOther_p, gdal_ogrspatialreference_ce)
      == FAILURE) {
    return;
  }

  poOther_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(poOther_p);
  poOther = poOther_obj->spatialreference;

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_BOOL(spatialreference->IsSameGeogCS(poOther));
}

PHP_METHOD(OGRSpatialReference, IsSame)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  zval *poOther_p;
  OGRSpatialReference *poOther;
  php_ogrspatialreference_object *poOther_obj;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &poOther_p, gdal_ogrspatialreference_ce)
      == FAILURE) {
    return;
  }

  poOther_obj = (php_ogrspatialreference_object*)
    zend_object_store_get_object(poOther_p);
  poOther = poOther_obj->spatialreference;

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  RETURN_BOOL(spatialreference->IsSame(poOther));
}

PHP_METHOD(OGRSpatialReference, Clear)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;
  spatialreference->Clear();
}

PHP_METHOD(OGRSpatialReference, SetLocalCS)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->SetLocalCS(str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, SetProjCS)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->SetProjCS(str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, SetProjection)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->SetProjection(str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, SetWellKnownGeogCS)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->SetWellKnownGeogCS(str);
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, SetFromUserInput)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *str;
  int str_len;
  int err;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &str, &str_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->SetFromUserInput(str);
  RETURN_LONG(err);
}


PHP_METHOD(OGRSpatialReference, AutoIdentifyEPSG)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  int err;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  err = spatialreference->AutoIdentifyEPSG();
  RETURN_LONG(err);
}

PHP_METHOD(OGRSpatialReference, GetEPSGGeogCS)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  int code;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  code = spatialreference->GetEPSGGeogCS();
  RETURN_LONG(code);
}

PHP_METHOD(OGRSpatialReference, GetAuthorityCode)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *pszTargetKey = NULL;
  int pszTargetKey_len;
  char *str;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"|s",
                            &pszTargetKey, &pszTargetKey_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  str = (char *)spatialreference->GetAuthorityCode(pszTargetKey);
  if (str) {
    RETURN_STRING(str, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(OGRSpatialReference, GetAuthorityName)
{
  OGRSpatialReference *spatialreference;
  php_ogrspatialreference_object *obj;
  char *pszTargetKey = NULL;
  int pszTargetKey_len;
  char *str;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"|s",
                            &pszTargetKey, &pszTargetKey_len) == FAILURE) {
    return;
  }

  obj = (php_ogrspatialreference_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  spatialreference = obj->spatialreference;

  str = (char *)spatialreference->GetAuthorityName(pszTargetKey);
  if (str) {
    RETURN_STRING(str, 1);
  } else {
    RETURN_NULL();
  }
}

//
// PHP stuff
//

function_entry ogrspatialreference_methods[] = {
  PHP_ME(OGRSpatialReference, __construct, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Reference, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Dereference, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, GetReferenceCount, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Release, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Clone, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, CloneGeogCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, exportToWkt, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, exportToPrettyWkt, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, exportToProj4, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, exportToPCI, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, exportToUSGS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, exportToXML, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, exportToPanorama, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, exportToERM, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, exportToMICoordSys, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, importFromWkt, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, importFromProj4, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, importFromEPSG, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, importFromEPSGA, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromESRI, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromPCI, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromUSGS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromPanorama, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromOzi, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromWMSAUTO, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromXML, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromDict, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromURN, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromERM, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromUrl, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, importFromMICoordSys, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, morphToESRI, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, morphFromESRI, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Validate, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, StripCTParms, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, StripVertical, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, FixupOrdering, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Fixup, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, EPSGTreatsAsLatLong, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetAxis, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetAxes, NULL, ZEND_ACC_PUBLIC)
  // // OGR_SRSNode-based methods are omitted
  // PHP_ME(OGRSpatialReference, SetLinearUnitsAndUpdateParameters, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetLinearUnits, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetLinearUnits, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetAngularUnits, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetAngularUnits, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetPrimeMeridian, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, IsGeographic, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, IsProjected, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, IsLocal, NULL, ZEND_ACC_PUBLIC)
  // OGRSpatialReference::IsVertical not in ogr_spatialref.h
  PHP_ME(OGRSpatialReference, IsSameGeogCS, NULL, ZEND_ACC_PUBLIC)
  // OGRSpatialReference::IsSameVertCS not in ogr_spatialref.h
  PHP_ME(OGRSpatialReference, IsSame, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, Clear, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, SetLocalCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, SetProjCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, SetProjection, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGeogCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, SetWellKnownGeogCS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, CopyGeogCSFrom, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, SetFromUserInput, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTOWGS84, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetTOWGS84, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetSemiMajor, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetSemiMinor, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetInvFlattening, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetAuthority, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, AutoIdentifyEPSG, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, GetEPSGGeogCS, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, GetAuthorityCode, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRSpatialReference, GetAuthorityName, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetExtension, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetExtension, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, FindProjParm, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetProjParm, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetProjParm, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetNormProjParm, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetNormProjParm, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, IsAngularParameter, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, IsLongitudeParameter, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, IsLinearParameter, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetACEA, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetAE, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetBonne, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetCEA, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetCS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEC, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEckert, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEckertIV, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEckertVI, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEquirectangular, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetEquirectangular2, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGEOS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGH, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGaussSchreiberTMercator, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetGnomonic, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetHOM, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetHOM2PNO, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetIWMPolyconic, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetKrovak, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetLAEA, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetLCC, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetLCC1SP, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetLCCB, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetMC, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetMercator, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetMercator2SP, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetMollweide, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetNZMG, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetOS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetOrthographic, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetPolyconic, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetPS, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetRobinson, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetSinusoidal, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetStereographic, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetSOC, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTM, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTMVariant, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTMG, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTMSO, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetTPED, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetVDG, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetUTM, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, GetUTMZone, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetWagner, NULL, ZEND_ACC_PUBLIC)
  // PHP_ME(OGRSpatialReference, SetStatePlane, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_ogrspatialreference_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRSpatialReference", ogrspatialreference_methods);
  gdal_ogrspatialreference_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrspatialreference_ce->create_object = ogrspatialreference_create_handler;
  memcpy(&ogrspatialreference_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrspatialreference_object_handlers.clone_obj = NULL;
}
