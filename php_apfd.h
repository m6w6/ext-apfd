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

#ifndef PHP_APFD_H
#define PHP_APFD_H

extern zend_module_entry apfd_module_entry;
#define phpext_apfd_ptr &apfd_module_entry

#define PHP_APFD_VERSION "1.0.0RC1"

#ifdef PHP_WIN32
#	define PHP_APFD_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_APFD_API extern __attribute__ ((visibility("default")))
#else
#	define PHP_APFD_API extern
#endif

#ifdef ZTS
#	include "TSRM.h"
#endif

#endif	/* PHP_APFD_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
