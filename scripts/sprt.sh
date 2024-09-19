#!/bin/bash

Help()
{
    echo "Script for automating self-play SPRT."
    echo
    echo "Syntax: sprt.sh <eng1> <eng2> [-b|r|h]"
    echo "- Provide version number or name as arguments"
    echo "Options:"
    echo "b:     Blitz TC, 10s + 0.1s increment"
    echo "r:     Rapid TC, 40 moves in 60 seconds"
    echo "h:     Print this Help"
    echo "By defualt, 'sprt' behaves as if '-b' were specified."
    exit 1
}


tc="inf/10+0.25"

while getopts ":brh" option; do
    case $option in
        b)
            tc="inf/10+0.2";;
        r)
            tc="40/60";;
        h)
            Help
            exit;;
        \?)
            echo "Error, invalid option."
            Help
            exit 1;;
    esac
done

PROJECT_PATH=/home/luka/Moss
ENGINES_PATH=$PROJECT_PATH/bin
dt=$(date +%s)

new_engine_version=$1
old_engine_version=$2

new_engine_name=$(ls $PROJECT_PATH/bin | grep $new_engine_version -m 1)
old_engine_name=$(ls $PROJECT_PATH/bin | grep $old_engine_version -m 1)

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
    -debug \
    -each proto=uci tc=$tc \
    -rounds 2000 \
    -sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 \
    -concurrency 4 \
    -ratinginterval 10 \
    -pgnout $LOGS_PATH/$new_engine_name-$old_engine_name.pgn \
    | tee $LOGS_PATH/$new_engine_name-$old_engine_name-results.txt

