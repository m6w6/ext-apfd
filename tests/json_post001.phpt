--TEST--
apfd - json_post PUT
--EXTENSIONS--
apfd
json_post
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
