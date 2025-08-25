#!/usr/bin/env php
<?php echo "# generated file; do not edit!\n"; ?>

name: ci
on:
  workflow_dispatch:
  push:
  pull_request:

jobs:
<?php

$cur = "8.4";
$gen = include __DIR__ . "/ci/gen-matrix.php";
$job = $gen->github([
"old-matrix" => [
	"PHP" => ["5.6", "7.4", "8.0", "8.1", "8.2", "8.3"],
	"enable_debug" => "yes",
	"enable_maintainer_zts" => "yes",
],
"master" => [
	"PHP" => "master",
	"enable_debug" => "yes",
	"enable_zts" => "yes",
],
"cur-dbg-zts" => [
	"PHP" => $cur,
	"enable_debug",
	"enable_zts",
],
"cur-cov" => [
	"CFLAGS" => "-O0 -g --coverage",
	"CXXFLAGS" => "-O0 -g --coverage",
	"PHP" => $cur,
]]);
foreach ($job as $id => $env) {
    printf("  %s:\n", $id);
    printf("    name: %s\n", $id);
    if ($env["PHP"] == "master") {
        printf("    continue-on-error: true\n");
    }
    printf("    env:\n");
    foreach ($env as $key => $val) {
        printf("      %s: \"%s\"\n", $key, $val);
    }
?>
    runs-on: ubuntu-20.04
    steps:
      - uses: actions/checkout@v2
        with:
          submodules: true
      - name: Install
        run: |
          sudo apt-get install -y \
            php-cli \
            php-pear \
            re2c
      - name: Prepare
        run: |
          make -f scripts/ci/Makefile php || make -f scripts/ci/Makefile clean php
      - name: Build
        run: |
          make -f scripts/ci/Makefile ext PECL=apfd
      - name: Test
        run: |
          make -f scripts/ci/Makefile test
<?php if (isset($env["CFLAGS"]) && strpos($env["CFLAGS"], "--coverage") != false) : ?>
      - name: Coverage
        if: success()
        uses: codecov/codecov-action@v5
        with:
          directory: src
<?php endif; ?>

<?php
}
