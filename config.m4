PHP_ARG_ENABLE(apfd, whether to enable apfd support,
[  --enable-apfd      Enable always-populate-form-data support])

if test "$PHP_APFD" != "no"; then
  PHP_NEW_EXTENSION(apfd, php_apfd.c, $ext_shared)
fi
