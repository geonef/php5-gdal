//
// examples.cc
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
// http://docstore.mik.ua/orelly/webprog/php/ch14_06.htm
// http://devzone.zend.com/article/4486 (c++ example)
// http://devzone.zend.com/article/1022 (very good!)
// http://talks.somabo.de/200903_montreal_php_extension_writing.pdf (good slideshow)

//init zval*
MAKE_STD_ZVAL(zval*);

// set string in zval
ZVAL_STRINGL(zval*, str, len, /* 0 ou 1 ..? */);

// set object property
std_object_handlers.write_property(zval* /*object*/, &z_key, query_string TSRMLS_CC);

// .. supprime la ref ?
zval_ptr_dtor(zval**);

// PDO
static void pdo_stmt_construct(pdo_stmt_t *stmt, zval *object, zend_class_entry *dbstmt_ce, zval *ctor_args TSRMLS_DC) /* {{{ */
{
  zval *query_string;
  zval z_key;

  MAKE_STD_ZVAL(query_string);
  ZVAL_STRINGL(query_string, stmt->query_string, stmt->query_stringlen, 1);
  ZVAL_STRINGL(&z_key, "queryString", sizeof("queryString")-1, 0);
  std_object_handlers.write_property(object, &z_key, query_string TSRMLS_CC);
  zval_ptr_dtor(&query_string);

  if (dbstmt_ce->constructor) {
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
    zval *retval;

    fci.size = sizeof(zend_fcall_info);
    fci.function_table = &dbstmt_ce->function_table;
    fci.function_name = NULL;
    fci.object_ptr = object;
    fci.symbol_table = NULL;
    fci.retval_ptr_ptr = &retval;
    if (ctor_args) {
      HashTable *ht = Z_ARRVAL_P(ctor_args);
      Bucket *p;

      fci.param_count = 0;
      fci.params = safe_emalloc(sizeof(zval*), ht->nNumOfElements, 0);
      p = ht->pListHead;
      while (p != NULL) {
        fci.params[fci.param_count++] = (zval**)p->pData;
        p = p->pListNext;
      }
    } else {
      fci.param_count = 0;
      fci.params = NULL;
    }
    fci.no_separation = 1;

    fcc.initialized = 1;
    fcc.function_handler = dbstmt_ce->constructor;
    fcc.calling_scope = EG(scope);
    fcc.called_scope = Z_OBJCE_P(object);
    fcc.object_ptr = object;

    if (zend_call_function(&fci, &fcc TSRMLS_CC) == FAILURE) {
      zval_dtor(object);
      ZVAL_NULL(object);
      object = NULL; /* marks failure */
    } else {
      zval_ptr_dtor(&retval);
    }

    if (fci.params) {
      efree(fci.params);
    }
  }
}
