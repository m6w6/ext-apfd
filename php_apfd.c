/*
    +--------------------------------------------------------------------+
    | PECL :: apfd                                                       |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2015, Michael Wallner <mike@php.net>                 |
    +--------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "SAPI.h"
#include "rfc1867.h"
#include "php_apfd.h"

#if PHP_VERSION_ID >= 70000

struct apfd {
	zval post;
	zval files;
};

#define APFD_SG(t) &PG(http_globals)[t]
static inline sapi_post_entry *apfd_get_post_entry(const char *ct_str, size_t ct_len)
{
	return zend_hash_str_find_ptr(&SG(known_post_content_types), ct_str, ct_len);
}

static inline void apfd_backup(struct apfd *apfd)
{
	if (SG(rfc1867_uploaded_files)) {
		destroy_uploaded_files_hash();
	}

	zval_ptr_dtor(APFD_SG(TRACK_VARS_POST));
	array_init(&apfd->post);
	ZVAL_COPY_VALUE(APFD_SG(TRACK_VARS_POST), &apfd->post);

	zval_ptr_dtor(APFD_SG(TRACK_VARS_FILES));
	array_init(&apfd->files);
	ZVAL_COPY_VALUE(APFD_SG(TRACK_VARS_FILES), &apfd->files);
}

static inline void apfd_update(struct apfd *apfd)
{
	zend_hash_str_update(&EG(symbol_table), "_POST", sizeof("_POST")-1, APFD_SG(TRACK_VARS_POST));
	Z_TRY_ADDREF_P(APFD_SG(TRACK_VARS_POST));

	zend_hash_str_update(&EG(symbol_table), "_FILES", sizeof("_FILES")-1, APFD_SG(TRACK_VARS_FILES));
	Z_TRY_ADDREF_P(APFD_SG(TRACK_VARS_FILES));
}

#else

struct apfd {
	zval *post;
	zval *files;
};

#define APFD_SG(t) PG(http_globals)[t]
static inline sapi_post_entry *apfd_get_post_entry(const char *ct_str, size_t ct_len TSRMLS_DC)
{
	sapi_post_entry *post_entry;

	if (SUCCESS == zend_hash_find(&SG(known_post_content_types), ct_str, ct_len+1, (void *) &post_entry)) {
		return post_entry;
	}
	return NULL;
}

static inline void apfd_backup(struct apfd *apfd TSRMLS_DC)
{
	apfd->post = APFD_SG(TRACK_VARS_POST);
	apfd->files = APFD_SG(TRACK_VARS_FILES);
}

static inline void apfd_update(struct apfd *apfd TSRMLS_DC)
{
	if (apfd->files != APFD_SG(TRACK_VARS_FILES) && APFD_SG(TRACK_VARS_FILES)) {
		Z_ADDREF_P(APFD_SG(TRACK_VARS_FILES));
		zend_hash_update(&EG(symbol_table), "_FILES", sizeof("_FILES"), &APFD_SG(TRACK_VARS_FILES), sizeof(zval *), NULL);
		if (apfd->files) {
			zval_ptr_dtor(&apfd->files);
		}
	}
}
#endif

PHP_RINIT_FUNCTION(apfd)
{
#ifndef TSRMLS_C
# define TSRMLS_C
# define TSRMLS_CC
#endif
	/* populate form data on non-POST requests */
	if (SG(request_info).request_method && strcasecmp(SG(request_info).request_method, "POST") && SG(request_info).content_type && *SG(request_info).content_type) {
		char *ct_str, *ct_dup = estrdup(SG(request_info).content_type);
		size_t ct_end = strcspn(ct_dup, ";, ");
		sapi_post_entry *post_entry = NULL;

		SG(request_info).content_type_dup = ct_dup;

		ct_str = zend_str_tolower_dup(ct_dup, ct_end);
		if ((post_entry = apfd_get_post_entry(ct_str, ct_end TSRMLS_CC))) {
			struct apfd apfd;

			apfd_backup(&apfd TSRMLS_CC);

			SG(request_info).post_entry = post_entry;

			if (post_entry->post_reader) {
				post_entry->post_reader(TSRMLS_C);
			}

			if (sapi_module.default_post_reader) {
				sapi_module.default_post_reader(TSRMLS_C);
			}

			sapi_handle_post(APFD_SG(TRACK_VARS_POST) TSRMLS_CC);

			apfd_update(&apfd TSRMLS_CC);
		}
		efree(ct_str);

		if (SG(request_info).content_type_dup) {
			efree(SG(request_info).content_type_dup);
			SG(request_info).content_type_dup = NULL;
		}
	}

	return SUCCESS;
}

PHP_MINFO_FUNCTION(apfd)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "apfd support", "enabled");
	php_info_print_table_end();
}

const zend_function_entry apfd_functions[] = {
	{0}
};

zend_module_entry apfd_module_entry = {
	STANDARD_MODULE_HEADER,
	"apfd",
	apfd_functions,
	NULL,
	NULL,
	PHP_RINIT(apfd),
	NULL,
	PHP_MINFO(apfd),
	PHP_APFD_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_APFD
ZEND_GET_MODULE(apfd)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
