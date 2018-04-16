#!/bin/bash

# Creates a header file that contains a base64 encoded xzipped representation of
# the Unifont .ttf font. To use this tool, you will need to download the Unifont
# TrueType font from http://unifoundry.com/unifont.html

usage() {
    echo "Generates a unifont embedded font header file from a .ttf file." 1>&2
    echo "Usage: $0 -f 'path/to/font.ttf' -o 'output.h'" 1>&2
    exit 1
}

# Read in arguments
while getopts ":f:o:" o; do
    case "${o}" in
        f)
            ttf=${OPTARG}
            ;;
        o)
            output=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

# If any arguments weren't supplied, exit
if [ -z "${ttf}" ] || [ -z "${output}" ]; then
    usage
fi

# Create the start of the header file
cat > "${output}" <<EOF
/*
 * If you've found this file whilst browsing the source code, look away. This
 * is one nasty file, and it would be better if nobody knew that this existed.
 *
 * It's a base64 encoded .ttf.xz file containing the Unifont Plane 0 font. This
 * serves as a font fallback for if nothing else is usable, as this font
 * supports _every_ Unicode character that this program is likely to ever see.
 */
#ifndef _UNIFONT_EMBEDDED_FONT_H
#define _UNIFONT_EMBEDDED_FONT_H

char _UNIFONT_B64_ENCODED[] =
EOF

# Create the b64 encoded .tar.xz file, and encase each line with quotes
xz --stdout -9 "${ttf}" | base64 | sed 's/.*/"&"/' >> "${output}"

# Finish the job...
echo ";" >> "${output}"
