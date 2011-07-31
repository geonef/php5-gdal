//
// ogrcoordtransform.h
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

#ifndef PHP_OGRCOORDTRANSFORM_H
#define PHP_OGRCOORDTRANSFORM_H

#include <ogr_spatialref.h>
#include "php.h"

extern zend_class_entry *gdal_ogrcoordtransform_ce;

struct php_ogrcoordtransform_object {
  zend_object std;
  OGRCoordinateTransformation *coordtransform;
};

void php_gdal_ogrcoordtransform_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRCOORDTRANSFORM_H */
