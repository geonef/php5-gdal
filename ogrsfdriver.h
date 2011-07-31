//
// ogrsfdriver.h
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

#ifndef OGRSFDRIVER_H
#define OGRSFDRIVER_H

#include "php.h"
#include "ogrsf_frmts.h"

extern zend_class_entry *gdal_ogrsfdriver_ce;

struct driver_object {
  zend_object std;
  OGRSFDriver *driver;
};

void php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS);

#endif /* OGRSFDRIVER_H */
