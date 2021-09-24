--TEST--
apfd - json_post PUT
--SKIPIF--
<?php
if (!extension_loaded("apfd") || !extension_loaded("json_post"))
	die("skip need apfd and json_post support\n");
?>
--PUT--
Content-type: application/json

{"test": "json_post"}
--FILE--
TEST
<?php
var_dump([$_SERVER["REQUEST_METHOD"] => $_POST]);
?>
DONE
--EXPECT--
TEST
array(1) {
  ["PUT"]=>
  array(1) {
    ["test"]=>
    string(9) "json_post"
  }
}
DONE
