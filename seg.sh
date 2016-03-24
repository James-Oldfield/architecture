#!/bin/sh

# First, clear the image output dir if already populated
if [ "$(ls "./bin/data/seg"*)" ]; then
  echo "cleaning up old segments..."
  rm -r ./bin/data/seg*
fi

# Run either the command line app or make and make run.
if [ "$1" = "default" ]; then
  echo "Running command line project with default images..."
  open -n ./bin/./creativeProject.app/ --args $2 $3 http://www.tekuto.com/wp-content/themes/tekuto2nd/images/topmain/toruso01.jpg?=20151006 long.jpg
elif [ "$1" = "interior" ]; then
  echo "Running command line project with interior images..."
  open -n ./bin/./creativeProject.app/ --args $2 $3 https://s-media-cache-ak0.pinimg.com/564x/67/8a/ed/678aed334b030344482dc84f24063552.jpg http://forums.androidcentral.com/attachments/photo-contests/110643d1396484236t-weekly-photo-contest-architecture-2014-04-03-02.01.44-1.jpg
elif [ "$1" = "inex" ]; then
  echo "Running command line project with interior images..."
  open -n ./bin/./creativeProject.app/ --args $2 $3 tek.jpg home-cropped.jpg
elif [ "$1" = "train" ]; then
  echo "Running command line project with interior images..."
  open -n ./bin/./creativeProject.app/ --args $2 $3 http://forums.androidcentral.com/attachments/photo-contests/110643d1396484236t-weekly-photo-contest-architecture-2014-04-03-02.01.44-1.jpg http://41.media.tumblr.com/tumblr_m57nhfQXI41ruxpd8o5_1280.jpg 
elif [ "$1" = "brutal" ]; then
  echo "Running command line project with brutal-house images..."
  open -n ./bin/./creativeProject.app/ --args 155 160 https://pbs.twimg.com/media/CePL_W4WwAUG1Tr.jpg https://pbs.twimg.com/media/CeAzXloXEAIXTx4.jpg
else
  make && make run
fi
