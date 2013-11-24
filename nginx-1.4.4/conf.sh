#!/bin/sh

./configure --prefix=/Users/tanxr/study/nginx_annotation/nginx \
	--with-pcre=/Users/tanxr/Documents/unix/pcre-8.33 \
 	--with-openssl=/Users/tanxr/Documents/unix/openssl-1.0.1e \
 	--with-zlib=/Users/tanxr/Documents/unix/zlib-1.2.8 \
 	--user=tanxr --group=staff \
 	--with-debug \
	--add-module=/Users/tanxr/study/nginx_annotation/mytest 
