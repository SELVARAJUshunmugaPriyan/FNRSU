#!/bin/bash
dockerSt="docker stop ";
for var in "$(docker ps -q --filter="status=running")"
do
	echo $dockerSt$var;
	eval $dockerSt$var;
done;
dockerRm="docker rm ";
for var in "$(docker ps -aq)"
do
	echo $dockerRm$var;
	eval $dockerRm$var;
done;
