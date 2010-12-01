PHP_ARG_ENABLE(gdal,
    [whether to enable GDAL extension support],
    [ --enable-gdal   Enable GDAL extension support])

if test "$PHP_GDAL" != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(GDAL_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, GDAL_SHARED_LIBADD)
    PHP_NEW_EXTENSION(gdal, gdal.cc ogrlayer.cc, $ext_shared)
fi
