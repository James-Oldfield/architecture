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
  open -n ./bin/./creativeProject.app/ --args 190 190 brut2.jpg brut1.jpg 0 1
elif [ "$1" = "shardSound" ]; then
  open -n ./bin/./creativeProject.app/ --args 195 195 shard.jpg shard.jpg 0 1
elif [ "$1" = "knightSound" ]; then
  open -n ./bin/./creativeProject.app/ --args 200 195 shard.jpg shard.jpg 0 1
elif [ "$1" = "shardBKSound" ]; then
  open -n ./bin/./creativeProject.app/ --args 200 225 shard.jpg iphone/bk2.jpg 0 1
elif [ "$1" = "camdenSound" ]; then
  open -n ./bin/./creativeProject.app/ --args 140 315 straight2.jpg camden.jpg 0 1
else
  make && make run
fi
