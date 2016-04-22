# architecture

An openframeworks-based project attempting to recreate styles of architecture from other epochs and styles using openCV.

## To run:

**command-line branch**

First run `make`.

Either run the app yourself, with something like:

`open -n ./bin/./creativeProject.app/ --args ${THRESH1} ${THRESH2} ${PHOTO1} ${PHOTO2} ${RECURSIVE} ${SOUND}`

Arguments: 

param | type | function
--- | --- | ---
THRESH1 | int | Threshold to use for Hough lines for the first photo.
THRESH2 | int | Ditto above but for second photo.
PHOTO1 | string | URL to the image. Can be absolute or relative.
PHOTO2 | string | Ditto above for photo 2.
RECURSIVE | int | Either `0` or `1`. 1 searches own architecture's segments recursively for replacements, whereas passing `1` doesn't.
SOUND | int | Either `0` or `1`. Use sound or not? N.B: Sound file must compliment second photo's name

e.g:

`open -n ./bin/./creativeProject.app/ --args 160 185 long.jpg https://pbs.twimg.com/media/Cc-xdt-W0AAo27u.jpg 0 0`

Or use the bash file:

`./seg.sh ${ALIAS}`