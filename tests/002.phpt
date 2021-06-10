--TEST--
apfd
--SKIPIF--
<?php
extension_loaded("apfd") or die("skip need apfd support\n");
?>
--PUT--
Content-Type: multipart/form-data; boundary=----------------------------6e182425881c
------------------------------6e182425881c
Content-Disposition: form-data; name="LICENSE"; filename="LICENSE"
Content-Type: application/octet-stream

Copyright (c) 2011-2012, Michael Wallner <mike@iworks.at>.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


------------------------------6e182425881c
Content-Disposition: form-data; name="composer"; filename="composer.json"
Content-Type: application/octet-stream

{
    "name": "m6w6/autocracy",
    "type": "library",
    "description": "http\\Controller preserves your autocracy",
    "keywords": ["http", "controller", "pecl", "pecl_http"],
    "homepage": "http://github.com/m6w6/autocracy",
    "license": "BSD-2",
    "authors": [
        {
            "name": "Michael Wallner",
            "email": "mike@php.net"
        }
    ],
    "require": {
        "php": ">=5.4.0",
        "pecl/pecl_http": "2.*"
    },
    "autoload": {
        "psr-0": {
            "http\\Controller": "lib"
        }
    }
}

------------------------------6e182425881c
Content-Disposition: form-data; name="user"

mike
------------------------------6e182425881c--
--FILE--
<?php

foreach($_FILES as $i => $v) {
	unset($_FILES[$i]['full_path']); // only in 8.1
}
var_dump($_POST, $_FILES);

?>
--EXPECTF--
array(1) {
  ["user"]=>
  string(4) "mike"
}
array(2) {
  ["LICENSE"]=>
  array(5) {
    ["name"]=>
    string(7) "LICENSE"
    ["type"]=>
    string(24) "application/octet-stream"
    ["tmp_name"]=>
    string(%d) "%sphp%s"
    ["error"]=>
    int(0)
    ["size"]=>
    int(1340)
  }
  ["composer"]=>
  array(5) {
    ["name"]=>
    string(13) "composer.json"
    ["type"]=>
    string(24) "application/octet-stream"
    ["tmp_name"]=>
    string(%d) "%sphp%s"
    ["error"]=>
    int(0)
    ["size"]=>
    int(550)
  }
}
