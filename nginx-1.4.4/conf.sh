#!/bin/sh

system=`uname`

if [ $system = "Linux" ]; then
	echo "linux platform"
	./configure --prefix=/home/tanxr/workspace/nginx_annotation/nginx \
			--with-pcre=/home/tanxr/workspace/nginx_annotation/pcre-8.33 \
			--with-openssl=/home/tanxr/workspace/nginx_annotation/openssl-1.0.1e \
			--with-zlib=/home/tanxr/workspace/nginx_annotation/zlib-1.2.8 \
			--user=tanxr --group=tanxr \
			--with-debug \
			--add-module=/home/tanxr/workspace/nginx_annotation/mytest 
fi

if [ $system = "Darwin" ]; then
	echo "mac platform"

	./configure --prefix=/Users/tanxr/study/nginx_annotation/nginx \
		--with-pcre=/Users/tanxr/Documents/unix/pcre-8.33 \
		--with-openssl=/Users/tanxr/Documents/unix/openssl-1.0.1e \
		--with-zlib=/Users/tanxr/Documents/unix/zlib-1.2.8 \
		--user=tanxr --group=staff \
		--with-debug \
		--add-module=/Users/tanxr/study/nginx_annotation/mytest 
fi
