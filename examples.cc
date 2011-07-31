//
// examples.cc
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
