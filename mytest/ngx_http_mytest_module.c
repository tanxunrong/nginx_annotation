
/*
 * Copyright (C) tanxrong(at)gmail.com
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
// #include <ngx_http_mytest_module.h>

typedef struct  
{
    ngx_int_t test_num;
    ngx_str_t test_str;
    ngx_int_t repeat;
}ngx_http_mytest_conf_t;

static char* ngx_http_mytest(ngx_conf_t *cf,ngx_command_t *cmd,void *conf);
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *rq);
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf);
static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("mytest"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF,
        ngx_http_mytest,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t,repeat),
        NULL
    },
    {
        ngx_string("test_num"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t,test_num),
        NULL
    },
    {
        ngx_string("test_str"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_mytest_conf_t,test_str),
        NULL
    },
};

//HTTP 框架初始化时的回调方法
static ngx_http_module_t ngx_http_mytest_module_ctx = {
	NULL,/*pre configuration*/
	NULL,/*post configuration*/
	NULL,/*create main config*/
	NULL,/*init main config*/
	NULL,/*create server config*/
	NULL,/*merge server config*/
	ngx_http_mytest_create_loc_conf,/*create location config*/
	NULL,/*merge location config*/
};

ngx_module_t ngx_http_mytest_module = {
	NGX_MODULE_V1,                         //ctx_index,index,spare1-3,version(模块版本) 七个整数
	&ngx_http_mytest_module_ctx,
	ngx_http_mytest_commands,
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master,初始化master的回调方法，目前框架不调用*/
    NULL,                                  /* init module,在master中初始化所有模块时调用*/
    NULL,                                  /* init process，正常服务之前，worker进程初始化时调用*/
    NULL,                                  /* init thread ，未调用*/
    NULL,                                  /* exit thread ，未调用*/
    NULL,                                  /* exit process，worker退出前调用*/
    NULL,                                  /* exit master ，master退出前调用*/
    NGX_MODULE_V1_PADDING                  /*保留字段，spare_hook,未使用*/
};

static char* ngx_http_mytest(ngx_conf_t *cf,ngx_command_t *cmd,void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
    clcf->handler = ngx_http_mytest_handler;

  ngx_http_mytest_conf_t *mycf = (ngx_http_mytest_conf_t *)conf;
  ngx_str_t *value = cf->args->elts;
  if (cf->args->nelts == 1)
  {
    mycf->repeat = ngx_atoi(value[1].data,value[1].len);
    if (mycf->repeat == NGX_ERROR)
    {
        return "invalid repeat num";
    }
  }
  else
  {
    return "invalid repeat num";
  }

  return NGX_CONF_OK;
}

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *rq)
{
	if (!rq->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))
    {
        return NGX_HTTP_NOT_ALLOWED;
    }

    ngx_int_t rc = ngx_http_discard_request_body(rq);

    if(rc != NGX_OK)
    {
        return rc;
    }

    ngx_str_t type = ngx_string("test/plain");
    ngx_str_t response = ngx_string("Hello World!");
    rq->headers_out.status = NGX_HTTP_OK;
    rq->headers_out.content_length_n = response.len;
    rq->headers_out.content_type = type;

    rc = ngx_http_send_header(rq);
    if (rc == NGX_ERROR || rc > NGX_OK || rq->header_only)
    {
        return rc;
    }

    ngx_buf_t *buff;
    buff = ngx_create_temp_buf(rq->pool,response.len);
    if (buff == NULL)
    {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_memcpy(buff->pos,response.data,response.len);
    buff->last = buff->pos + response.len ;
    buff->last_buf = 1;

    ngx_chain_t out;
    out.buf = buff;
    out.next = NULL;

    return ngx_http_output_filter(rq,&out);
}

static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mytest_conf_t *mycf;
    mycf = (ngx_http_mytest_conf_t *) ngx_pcalloc(cf->pool,sizeof(ngx_http_mytest_conf_t));
    if (mycf == NULL)
    {
        return NULL;
    } 
    mycf->test_num = NGX_CONF_UNSET; 
    return mycf;
}