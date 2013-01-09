<?php

define('AMOUNT', $argv[1]);
define('ITERATIONS', $argv[2]);

$resources = [];
echo "Creating ".AMOUNT. " resources:\t";
$start = microtime(true);
for ($i = 0; $i < AMOUNT; ++$i) {
	$resources[] = resvsobj_new_resource();
}
echo microtime(true)-$start, "\n";

echo "Calling add " . ITERATIONS . "x/resource:\t";
$start = microtime(true);
for ($i = 0; $i < AMOUNT; ++$i) {
	for ($j = 0; $j < ITERATIONS; ++$j) {
		resvsobj_add($resources[$i]);
	}
}
echo microtime(true)-$start, "\n";

echo "Freeing all (unset of array):\t";
$start = microtime(true);
unset($resources);
echo microtime(true)-$start, "\n";


#######

$objects = [];
echo "Creating ".AMOUNT. " objects:\t";
$start = microtime(true);
for ($i = 0; $i < AMOUNT; ++$i) {
	$objects[] = new resvsobj();
}
echo microtime(true)-$start, "\n";

echo "Calling add " . ITERATIONS . "x/object:\t";
$start = microtime(true);
for ($i = 0; $i < AMOUNT; ++$i) {
	for ($j = 0; $j < ITERATIONS; ++$j) {
		$objects[$i]->add();
	}
}
echo microtime(true)-$start, "\n";

echo "Freeing all (unset of array):\t";
$start = microtime(true);
unset($objects);
echo microtime(true)-$start, "\n";

