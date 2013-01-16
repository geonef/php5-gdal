--TEST--
Test: OGRGeometry
--SKIPIF--
<?php if (!extension_loaded("gdal")) print "skip"; ?>
--FILE--
<?php
OGRRegisterAll();
$datasource = OGRSFDriverRegistrar::Open('./tests/data/points.shp', false);
if (!$datasource) {
    echo "Could not open shapefile\n";
    exit(1);
}
$layer = $datasource->GetLayer(0);
if (!$layer) {
    print "layer not found\n";
    exit(1);
}
$feature = $layer->GetNextFeature();
assert($feature);

$idxName = $feature->GetFieldIndex("name");
assert(!is_null($idxName));
echo $feature->getFieldAsString($idxName)."\n";

$geom = $feature->GetGeometryRef();
var_dump($geom);
echo "OGRGeometry->IsValid: " . $geom->IsValid() . "\n";
echo "OGRGeometry->ExportToWkt: " . $geom->ExportToWkt() . "\n";
?>
--EXPECT--
First Point
object(OGRGeometry)#4 (0) {
}
OGRGeometry->IsValid: 1
OGRGeometry->ExportToWkt: POINT (-28 35)
