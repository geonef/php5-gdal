
#include "php.h"
#include "php_gdal.h"
#include "ogrsf_frmts.h"
#include "ogrlayer.h"

zend_object_handlers ogrlayer_object_handlers;
zend_class_entry *ogrlayer_ce;

struct ogrlayer_object {
  zend_object std;
  OGRLayer *layer;
};

//
// Object methods
//

// PHP_METHOD(OGRLayer, __construct)
// {
//   long maxGear;
//   Car *car = NULL;
//   zval *object = getThis();

//   if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &maxGear) == FAILURE) {
//     RETURN_NULL();
//   }

//   car = new Car(maxGear);
//   car_object *obj = (car_object *)zend_object_store_get_object(object TSRMLS_CC);
//   obj->car = car;
// }

PHP_METHOD(OGRLayer, ResetReading)
{
}


//
// PHP stuff
//

function_entry ogrlayer_methods[] = {
  //PHP_ME(OGRLayer,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
  PHP_ME(OGRLayer,  ResetReading,    NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void ogrlayer_free_storage(void *object TSRMLS_DC)
{
  ogrlayer_object *obj = (ogrlayer_object *)object;
  delete obj->layer;

  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);

  efree(obj);
}

zend_object_value ogrlayer_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  ogrlayer_object *obj = (ogrlayer_object *)emalloc(sizeof(ogrlayer_object));
  memset(obj, 0, sizeof(ogrlayer_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrlayer_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrlayer_object_handlers;

  return retval;
}

void gdal_ogrlayer_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRLayer", ogrlayer_methods);
  ogrlayer_ce = zend_register_internal_class(&ce TSRMLS_CC);
  ogrlayer_ce->create_object = ogrlayer_create_handler;
  memcpy(&ogrlayer_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrlayer_object_handlers.clone_obj = NULL;
}
