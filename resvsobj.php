<?php

define('AMOUNT', $argv[1]);
define('ITERATIONS', $argv[2]);

// Allocate some memory and free it beforehand to be a bit fairer
for ($i = 0; $i < AMOUNT*2; ++$i) {
	$void[] = $i; 
}
unset($void);

#################

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
echo microtime(true)-$start, "\n\n";

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

