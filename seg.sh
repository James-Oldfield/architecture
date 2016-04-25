#!/bin/sh

# Run either the command line app or make and make run.
if [ "$1" = "default" ]; then
  open -n ./bin/./creativeProject.app/ --args $2 $3 http://www.tekuto.com/wp-content/themes/tekuto2nd/images/topmain/toruso01.jpg?=20151006 long.jpg 0 0
elif [ "$1" = "brutalCamden" ]; then
  open -n ./bin/./creativeProject.app/ --args 155 313 straight.jpg camden.jpg 0 0
elif [ "$1" = "brutal" ]; then
  open -n ./bin/./creativeProject.app/ --args 155 313 straight.jpg camden.jpg 0 0
elif [ "$1" = "shard" ]; then
  open -n ./bin/./creativeProject.app/ --args 200 200 shard.jpg shard.jpg 0 0

# SOUND PRESETS
elif [ "$1" = "brutSound" ]; then
  open -n ./bin/./creative_project.app/ --args 190 190 brut2.jpg brut1.jpg 0 1
elif [ "$1" = "shardSound" ]; then
  open -n ./bin/./creative_project.app/ --args 195 195 shard.jpg shard.jpg 0 1
elif [ "$1" = "knightSound" ]; then # 2 mins
  open -n ./bin/./creative_project.app/ --args 215 263 shard.jpg knight.jpg 0 1
elif [ "$1" = "shardBKSound" ]; then
  open -n ./bin/./creative_project.app/ --args 200 225 shard.jpg iphone/bk2.jpg 0 1
elif [ "$1" = "camdenSound" ]; then # 5:25 runtime.
  open -n ./bin/./creative_project.app/ --args 140 317 straight2.jpg camden.jpg 0 1
elif [ "$1" = "stacked" ]; then
  open -n ./bin/./creative_project.app/ --args 240 100 stackedMulti.jpg stackedGrey.jpg 0 0
elif [ "$1" = "stackedGold" ]; then
  open -n ./bin/./creative_project.app/ --args 175 240 stackedPlain.jpg stackedGold.jpg 0 1
elif [ "$1" = "gold" ]; then
  open -n ./bin/./creative_project.app/ --args 250 250 plain.jpg gold.jpg 0 1
elif [ "$1" = "shard6" ]; then # 6:47 run time.
  open -n ./bin/./creative_project.app/ --args 210 205 shard.jpg shard.jpg 0 1
elif [ "$1" = "gold6" ]; then # 6:00 + run time.
  open -n ./bin/./creative_project.app/ --args 185 235 plain.jpg gold.jpg 0 1
elif [ "$1" = "bk" ]; then # 2:30+ run time
  open -n ./bin/./creative_project.app/ --args 190 233 brut2.jpg bk2.jpg 0 1
else
  make && make run
fi
