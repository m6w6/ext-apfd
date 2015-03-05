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
#include "php_apfd.h"

PHP_RINIT_FUNCTION(apfd)
{
	/* populate form data on non-POST requests */
	if (SG(request_info).request_method && strcasecmp(SG(request_info).request_method, "POST") && SG(request_info).content_type && *SG(request_info).content_type) {
		char *ct_str = zend_str_tolower_dup(SG(request_info).content_type, strlen(SG(request_info).content_type));
		size_t ct_end = strcspn(ct_str, ";, ");
		sapi_post_entry *post_entry = NULL;
		char delim;

		SG(request_info).content_type_dup = ct_str;

		delim = ct_str[ct_end];
		ct_str[ct_end] = '\0';

		if (SUCCESS == zend_hash_find(&SG(known_post_content_types), ct_str, ct_end+1, (void *) &post_entry)) {
			zval *files = PG(http_globals)[TRACK_VARS_FILES];

			ct_str[ct_end] = delim;

			if (post_entry) {
				SG(request_info).post_entry = post_entry;

				if (post_entry->post_reader) {
					post_entry->post_reader(TSRMLS_C);
				}
			}

			if (sapi_module.default_post_reader) {
				sapi_module.default_post_reader(TSRMLS_C);
			}

			sapi_handle_post(PG(http_globals)[TRACK_VARS_POST] TSRMLS_CC);

			/*
			 * the rfc1867 handler is an awkward buddy
			 */
			if (files != PG(http_globals)[TRACK_VARS_FILES] && PG(http_globals)[TRACK_VARS_FILES]) {
				Z_ADDREF_P(PG(http_globals)[TRACK_VARS_FILES]);
				zend_hash_update(&EG(symbol_table), "_FILES", sizeof("_FILES"), &PG(http_globals)[TRACK_VARS_FILES], sizeof(zval *), NULL);
				if (files) {
					zval_ptr_dtor(&files);
				}
			}
		}

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
