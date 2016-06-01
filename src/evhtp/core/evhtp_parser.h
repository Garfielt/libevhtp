#ifndef __EVHTP_PARSER_H__
#define __EVHTP_PARSER_H__

#include "evhtp-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct evhtp_parser;

enum evhtp_type {
    evhtp_type_request = 0,
    evhtp_type_response
};

enum evhtp_scheme {
    evhtp_scheme_none = 0,
    evhtp_scheme_ftp,
    evhtp_scheme_http,
    evhtp_scheme_https,
    evhtp_scheme_nfs,
    evhtp_scheme_unknown
};

enum evhtp_method {
    evhtp_method_GET = 0,
    evhtp_method_HEAD,
    evhtp_method_POST,
    evhtp_method_PUT,
    evhtp_method_DELETE,
    evhtp_method_MKCOL,
    evhtp_method_MOVE,
    evhtp_method_OPTIONS,
    evhtp_method_PROPFIND,
    evhtp_method_PROPPATCH,
    evhtp_method_LOCK,
    evhtp_method_UNLOCK,
    evhtp_method_TRACE,
    evhtp_method_CONNECT, /* RFC 2616 */
    evhtp_method_PATCH,   /* RFC 5789 */
    evhtp_method_UNKNOWN,
};

enum evhtp_parser_error {
    htparse_error_none = 0,
    htparse_error_too_big,
    htparse_error_inval_method,
    htparse_error_inval_reqline,
    htparse_error_inval_schema,
    htparse_error_inval_proto,
    htparse_error_inval_ver,
    htparse_error_inval_hdr,
    htparse_error_inval_chunk_sz,
    htparse_error_inval_chunk,
    htparse_error_inval_state,
    htparse_error_user,
    htparse_error_status,
    htparse_error_generic
};

typedef struct evhtp_parser       evhtp_parser;
typedef struct evhtp_parser_hooks evhtp_parser_hooks;

typedef enum evhtp_scheme         evhtp_scheme;
typedef enum evhtp_method         evhtp_method;
typedef enum evhtp_type           evhtp_type;
typedef enum evhtp_parser_error   evhtp_parser_error;

typedef int (* evhtp_parser_hook)(evhtp_parser *);
typedef int (* evhtp_parser_data_hook)(evhtp_parser *, const char *, size_t);


struct evhtp_parser_hooks {
    evhtp_parser_hook      on_msg_begin;
    evhtp_parser_data_hook method;
    evhtp_parser_data_hook scheme;             /* called if scheme is found */
    evhtp_parser_data_hook host;               /* called if a host was in the request scheme */
    evhtp_parser_data_hook port;               /* called if a port was in the request scheme */
    evhtp_parser_data_hook path;               /* only the path of the uri */
    evhtp_parser_data_hook args;               /* only the arguments of the uri */
    evhtp_parser_data_hook uri;                /* the entire uri including path/args */
    evhtp_parser_hook      on_hdrs_begin;
    evhtp_parser_data_hook hdr_key;
    evhtp_parser_data_hook hdr_val;
    evhtp_parser_data_hook hostname;
    evhtp_parser_hook      on_hdrs_complete;
    evhtp_parser_hook      on_new_chunk;       /* called after parsed chunk octet */
    evhtp_parser_hook      on_chunk_complete;  /* called after single parsed chunk */
    evhtp_parser_hook      on_chunks_complete; /* called after all parsed chunks processed */
    evhtp_parser_data_hook body;
    evhtp_parser_hook      on_msg_complete;
};


EVHTP_EXPORT size_t             evhtp_parser_run(evhtp_parser *, evhtp_parser_hooks *, const char *, size_t);
EVHTP_EXPORT int                evhtp_parser_should_keep_alive(evhtp_parser * p);
EVHTP_EXPORT evhtp_scheme       evhtp_parser_get_scheme(evhtp_parser *);
EVHTP_EXPORT evhtp_method       evhtp_parser_get_method(evhtp_parser *);
EVHTP_EXPORT const char       * evhtp_parser_get_methodstr(evhtp_parser *);
EVHTP_EXPORT const char       * evhtp_parser_get_methodstr_m(evhtp_method);
EVHTP_EXPORT void               evhtp_parser_set_major(evhtp_parser *, unsigned char);
EVHTP_EXPORT void               evhtp_parser_set_minor(evhtp_parser *, unsigned char);
EVHTP_EXPORT unsigned char      evhtp_parser_get_major(evhtp_parser *);
EVHTP_EXPORT unsigned char      evhtp_parser_get_minor(evhtp_parser *);
EVHTP_EXPORT unsigned char      evhtp_parser_get_multipart(evhtp_parser *);
EVHTP_EXPORT unsigned int       evhtp_parser_get_status(evhtp_parser *);
EVHTP_EXPORT uint64_t           evhtp_parser_get_content_length(evhtp_parser *);
EVHTP_EXPORT uint64_t           evhtp_parser_get_content_pending(evhtp_parser *);
EVHTP_EXPORT uint64_t           evhtp_parser_get_total_bytes_read(evhtp_parser *);
EVHTP_EXPORT evhtp_parser_error evhtp_parser_get_error(evhtp_parser *);
EVHTP_EXPORT const char       * evhtp_parser_get_strerror(evhtp_parser *);
EVHTP_EXPORT void             * evhtp_parser_get_userdata(evhtp_parser *);
EVHTP_EXPORT void               evhtp_parser_set_userdata(evhtp_parser *, void *);
EVHTP_EXPORT void               evhtp_parser_init(evhtp_parser *, evhtp_type);
EVHTP_EXPORT evhtp_parser     * evhtp_parser_new(void);

#ifdef __cplusplus
}
#endif

#endif

