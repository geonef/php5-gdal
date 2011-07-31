//
// ogrfielddefn.h
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

#ifndef PHP_OGRFIELDDEFN_H
#define PHP_OGRFIELDDEFN_H

#include <ogr_feature.h>
#include "php.h"

extern zend_class_entry *gdal_ogrfielddefn_ce;

struct php_ogrfielddefn_object {
  zend_object std;
  OGRFieldDefn *fielddefn;
};

void php_gdal_ogrfielddefn_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRFIELDDEFN_H */
