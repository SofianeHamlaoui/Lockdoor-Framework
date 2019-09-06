/*
 * DIRB
 *
 * http_codes.h - http defines
 *
 */

#include "global.h"


// HTTP Code

#define HTTP_CONTINUE                      100
#define HTTP_SWITCHING_PROTOCOLS           101
#define HTTP_OK                            200
#define HTTP_CREATED                       201
#define HTTP_ACCEPTED                      202
#define HTTP_NON_AUTHORITATIVE             203
#define HTTP_NO_CONTENT                    204
#define HTTP_RESET_CONTENT                 205
#define HTTP_PARTIAL_CONTENT               206
#define HTTP_MULTIPLE_CHOICES              300
#define HTTP_MOVED_PERMANENTLY             301
#define HTTP_MOVED_TEMPORARILY             302
#define HTTP_SEE_OTHER                     303
#define HTTP_NOT_MODIFIED                  304
#define HTTP_USE_PROXY                     305
#define HTTP_BAD_REQUEST                   400
#define HTTP_UNAUTHORIZED                  401
#define HTTP_PAYMENT_REQUIRED              402
#define HTTP_FORBIDDEN                     403
#define HTTP_NOT_FOUND                     404
#define HTTP_METHOD_NOT_ALLOWED            405
#define HTTP_NOT_ACCEPTABLE                406
#define HTTP_PROXY_AUTHENTICATION_REQUIRED 407
#define HTTP_REQUEST_TIME_OUT              408
#define HTTP_CONFLICT                      409
#define HTTP_GONE                          410
#define HTTP_LENGTH_REQUIRED               411
#define HTTP_PRECONDITION_FAILED           412
#define HTTP_REQUEST_ENTITY_TOO_LARGE      413
#define HTTP_REQUEST_URI_TOO_LARGE         414
#define HTTP_UNSUPPORTED_MEDIA_TYPE        415
#define HTTP_INTERNAL_SERVER_ERROR         500
#define HTTP_NOT_IMPLEMENTED               501
#define HTTP_BAD_GATEWAY                   502
#define HTTP_SERVICE_UNAVAILABLE           503
#define HTTP_GATEWAY_TIME_OUT              504
#define HTTP_VERSION_NOT_SUPPORTED         505
#define HTTP_VARIANT_ALSO_VARIES           506


// HTTP Code Strings

static struct code code2message[] = {
  { HTTP_OK,                      "Ok" },
  { HTTP_MOVED_PERMANENTLY,       "Moved Permanently" },
  { HTTP_MOVED_TEMPORARILY,       "Moved Temporarily" },
  { HTTP_BAD_REQUEST,             "Bad Request"},
  { HTTP_UNAUTHORIZED,            "Auth Required" },
  { HTTP_FORBIDDEN,               "Forbidden" },
  { HTTP_NOT_FOUND,               "Not Found" },
  { HTTP_UNSUPPORTED_MEDIA_TYPE,  "Unsupported Media Type" },
  { HTTP_METHOD_NOT_ALLOWED,      "Method Not Allowed" },
  { HTTP_INTERNAL_SERVER_ERROR,   "Internal Server Error" },
  { HTTP_SERVICE_UNAVAILABLE,     "Service Temporarily Unavailable" },
  { 0xff,                         "Unknown"},
};

