#! /usr/bin/env bash

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Must be executed at the same level.

rep=$(pwd); [ "${0%/*}" = "." ]     || exit 1

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

./Build.sh                          || exit 1

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

cd ${rep}/Tutorials/Plugins
./Build.sh                          || exit 1

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

cd ${rep}/Spaghettis/Modules/T
./Build.sh                          || exit 1
./tests                             || exit 1
./Symbols.py                        || exit 1

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------
