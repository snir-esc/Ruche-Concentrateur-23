#!/bin/bash

IFS='x'

output1=$(i2cget -y 1 0x69 1)
echo "$output1"
output2=$(i2cget -y 1 0x69 2)
echo "$output2"

read -a UniteV <<< "$output1"
read -a DecimalV <<< "$output2"


echo "$((16#${UniteV[1]})).$((16#${DecimalV[1]}))V"

