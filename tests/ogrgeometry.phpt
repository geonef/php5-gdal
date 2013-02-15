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

function bool2string($b) {
    return $b ? "TRUE" : "FALSE";
}

$geom = $feature->GetGeometryRef();
var_dump($geom);
echo "OGRGeometry->IsValid: " . bool2string($geom->IsValid()) . "\n";
echo "OGRGeometry->GetGeometryName: " . $geom->GetGeometryName() . "\n";
echo "OGRGeometry->GetGeometryType: is wktPoint -> " . bool2string($geom->GetGeometryType() === wkbPoint) . "\n";
echo "OGRGeometry->ExportToWkt: " . $geom->ExportToWkt() . "\n";
echo "OGRGeometry->ExportToWkb (bin2hex): " . bin2hex($geom->ExportToWkb()) . "\n";
try {
    $geom->ExportToWkb(23552);
    echo "Passing illegal byteorder to OGRGeometry->ExportToWkb did not cause an exception\n";
}
catch (OGRException $e) {
    echo "OGRException on illegal byteorder caught.\n";
}
// wrap with json_(de|encode) to work around number formatting in different GDAL versions
// -28.000000 <-> -28.0
echo "OGRGeometry->ExportToJson: " . json_encode(json_decode($geom->ExportToJson())) . "\n";

echo "OGRGeometry->ExportToKML: " . $geom->ExportToKML() . "\n";
echo "OGRGeometry->ExportToGML: " . $geom->ExportToGML() . "\n";
echo "OGRGeometry->IsEmpty: " . bool2string($geom->IsEmpty()) . "\n";
?>
--EXPECT--
First Point
object(OGRGeometry)#4 (0) {
}
OGRGeometry->IsValid: TRUE
OGRGeometry->GetGeometryName: POINT
OGRGeometry->GetGeometryType: is wktPoint -> TRUE
OGRGeometry->ExportToWkt: POINT (-28 35)
OGRGeometry->ExportToWkb (bin2hex): 01010000000000000000003cc0000000000080414000
OGRException on illegal byteorder caught.
OGRGeometry->ExportToJson: {"type":"Point","coordinates":[-28,35]}
OGRGeometry->ExportToKML: <Point><coordinates>-28,35</coordinates></Point>
OGRGeometry->ExportToGML: <gml:Point><gml:coordinates>-28,35</gml:coordinates></gml:Point>
OGRGeometry->IsEmpty: FALSE
