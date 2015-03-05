--TEST--
apfd
--SKIPIF--
<?php
extension_loaded("apfd") or die("skip need apfd support\n");
?>
--PUT--
Content-Type: application/x-www-form-urlencoded
foo=bar&bar=foo
--FILE--
<?php

var_dump($_POST);

?>
--EXPECT--
array(2) {
  ["foo"]=>
  string(3) "bar"
  ["bar"]=>
  string(3) "foo"
}
