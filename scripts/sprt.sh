#!/bin/bash

PROJECT_PATH=/home/luka/lchess
ENGINES_PATH=$PROJECT_PATH/bin
dt=$(date +%s)

new_engine_version=$1
old_engine_version=$2

new_engine_name=$(ls $PROJECT_PATH/bin | grep $new_engine_version)
old_engine_name=$(ls $PROJECT_PATH/bin | grep $old_engine_version)

mkdir $PROJECT_PATH/logs/$new_engine_name-$old_engine_name-$dt
LOGS_PATH=$PROJECT_PATH/logs/$new_engine_name-$old_engine_name-$dt
touch $LOGS_PATH/$new_engine_name-$old_engine_name-results.txt

echo "Running SPRT between" $new_engine_name "and" $old_engine_name



new_engine_cmd=$ENGINES_PATH/$new_engine_name
old_engine_cmd=$ENGINES_PATH/$old_engine_name

echo "Using bins: " $new_engine_cmd "and" $old_engine_cmd

cutechess-cli \
    -engine cmd=$new_engine_cmd name=$new_engine_name \
    -engine cmd=$old_engine_cmd name=$old_engine_name \
    -each proto=uci tc=100/10 \
    -rounds 2000 \
    -sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 \
    -concurrency 4 \
    -ratinginterval 10 \
    -pgnout $LOGS_PATH/$new_engine_name-$old_engine_name.pgn \
    | tee $LOGS_PATH/$new_engine_name-$old_engine_name-results.txt

