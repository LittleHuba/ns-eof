#!/bin/bash
arr=($(ls | grep .job))
for i in "${arr[@]}";do
	subl "$i"
done
