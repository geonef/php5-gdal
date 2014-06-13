What is php5-gdal?
==================

php5-gdal is a PHP 5 module that provides PHP bindings to the GDAL/OGR library.
It was written and is maintained by the Geonef company, http://www.geonef.fr/.

This package is released under an X11/MIT style Open Source license. See LICENSE.txt file.

The build requires GDAL 1.7 or above.


This module is INCOMPLETE : roughly only half of the OGR library is supported
and very few classes of GDAL. Adding bindings is a bit repetitive, not so
difficult, so feel free to add the ones you need.

This module is UNSTABLE. Don't use it on production environment as it is.


BUILD INSTRUCTIONS
==================

Some builds are available already from the [builds/](builds/) directory. There may be some suitable for your Debian GNU/Linux system. Otherwise you can build it yourself by following the instructions below.

Build instructions for Debian GNU/Linux:
```
# apt-get install g++ php5-dev libgdal1-dev
# git clone https://github.com/geonef/php5-gdal.git && cd php5-gdal
# phpize
# ./configure
# make
# make install
# cat <<EOF >/etc/php5/conf.d/gdal.ini
; configuration for GDAL module
extension=gdal.so
EOF
# service apache2 graceful # if running mod-php5 for Apache2
# service php5-fpm reload # if running FastCGI PHP-FPM
# php -i | less # to check that extension "gdal" is enabled, you should see:
gdal

GDAL support => enabled
OGR support => enabled
GDAL version => GDAL 1.9.0, released 2011/12/29
GDAL drivers => VRT GTiff NITF RPFTOC ECRGTOC HFA SAR_CEOS CEOS JAXAPALSAR GFF ELAS AIG AAIGrid GRASSASCIIGrid SDTS OGDI DTED PNG JPEG MEM JDEM GIF BIGGIF ESAT BSB XPM BMP DIMAP AirSAR RS2 PCIDSK PCRaster ILWIS SGI SRTMHGT Leveller Terragen GMT netCDF HDF4 HDF4Image ISIS3 ISIS2 PDS TIL ERS JPEG2000 L1B FIT GRIB RMF WCS WMS MSGN RST INGR GSAG GSBG GS7BG COSAR TSX COASP R PNM DOQ1 DOQ2 ENVI EHdr GenBin PAux MFF MFF2 FujiBAS GSC FAST BT LAN CPG IDA NDF EIR DIPEx LCP GTX LOSLAS NTv2 ACE2 SNODAS RIK USGSDEM GXF DODS HTTP BAG HDF5 HDF5Image NWT_GRD NWT_GRC ADRG SRP BLX Rasterlite EPSILON PostGISRaster SAGA KMLSUPEROVERLAY XYZ HF2 PDF OZI CTG E00GRID ZMap NGSGEOID
OGR drivers => ESRI Shapefile MapInfo File UK .NTF SDTS TIGER S57 DGN VRT REC Memory BNA CSV NAS GML GPX LIBKML KML GeoJSON Interlis 1 Interlis 2 GMT SQLite DODS ODBC PGeo MSSQLSpatial OGDI PostgreSQL MySQL PCIDSK XPlane AVCBin AVCE00 DXF Geoconcept GeoRSS GPSTrackMaker VFK PGDump GPSBabel SUA OpenAir PDS WFS HTF AeronavFAA Geomedia EDIGEO GFT SVG CouchDB Idrisi ARCGEN SEGUKOOA SEGY XLS
```

For other distributions/OS, please adapt the instruction (the "apt-get install", mainly), feel free to share them through a GitHub pull request or email to to maintainer jf@geonef.fr. Thanks!

Build a Debian (.deb) package
-----------------------------
```
# cd [php5-gdal source dir]
# checkinstall --pkgname=php5-gdal --pkgversion="`date +'%Y%m%d'`" --requires=php5-common,libgdal1 --install=no --maintainer='JF Gigand <jf@geonef.fr>' --inspect --strip=no
## answer 'n' on question "Should I create a default set of package docs?"
## press ENTER to confirm
```

You should now see (where 20140613-1 is the version, named after today's date):
```
**********************************************************************

 Done. The new package has been saved to

 [php5-gdal source dir]/php5-gdal_20140613-1_amd64.deb
 You can install it in your system anytime using: 

      dpkg -i php5-gdal_20140613-1_amd64.deb

**********************************************************************
```


COMMUNITY
---------

The source is hosted on GitHub : https://github.com/geonef/php5-gdal
It was added recently and basic documentation is yet to be written.
