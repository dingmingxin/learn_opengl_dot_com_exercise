#!/bin/bash - 
#===============================================================================
#
#          FILE: gen_gtags.sh
# 
#         USAGE: ./gen_gtags.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 11/15/2017 19:35
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

TAGSYSTEM_ROOT=$HOME/dotfiles/config_tagsystem
GTAGS_CONF_FILE=$TAGSYSTEM_ROOT/gtags.conf
# CTAGS_CONF=$TAGSYSTEM_ROOT/ctags_lua.cnf

source_root=$(dirname $0)/src

tagfile=$source_root/GTAGS
filelist=$source_root/.gtags.files


if [[ -f $tagfile ]]; then
	gtags
fi

echo 'create gtags.files ... '

update_filelist(){
	if [ -f $filelist ]; then
		cat /dev/null > $filelist
	else
		touch $filelist
	fi
	find -L -E $source_root -type f \( -iregex ".*/*.c" -or -iregex ".*/*.cpp" -or -iregex ".*/*.h" \) > $filelist
}

update_filelist
gtags --gtagsconf $GTAGS_CONF_FILE --gtagslabel exuberant-ctags -f $filelist
# gtags --gtagsconf $GTAGS_CONF_FILE --gtagslabel exuberant-ctags -d GTAGS 



