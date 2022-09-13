#! /usr/bin/env python3

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Copyright (c) 2017 Jojo and others.

# < http://opensource.org/licenses/BSD-3-Clause >

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Simple script to check the symbols table consistency.

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

import glob

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

symbols = [ ]
result  = True
path    = "../spaghettis_core/Core"

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Use regex instead?

def findOrphanCount(file, name):
    count = 0
    with open(file, "r") as f:
        for line in f:
            count += line.count(name + " ")
            count += line.count(name + ";")
            count += line.count(name + ")")
            count += line.count(name + ",")
            count += line.count(name + "-")
    return count
            
def findOrphan(name):
    global result
    global path
    count = 0
    files = glob.glob(path + "/**/*.c", recursive=True) + glob.glob(path + "/**/*.h", recursive=True)
    for file in files:
        count += findOrphanCount(file, name)
    if count < 4:
        print("###", name, "/", count)
        result = False
    else:
        print(name)
        
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

def findOrphans():
    global symbols
    global result
    for name in symbols:
        findOrphan(name)
    if result:
        print("SUCCEEDED")
    else:
        print("WRONG")
        
def sortSymbols():
    global symbols
    t = symbols[:]
    symbols.sort()
    if t != symbols:
        print("WARNING: symbols not sorted!")
    
def fetchSymbols():
    global symbols
    global path
    with open(path + "/core/m_symbols.h", "r") as f:
        for line in f:
            if line.startswith("extern t_symbol *"):
                symbols.append(line[17:-2])

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

if __name__ == "__main__":
    fetchSymbols()
    sortSymbols()
    findOrphans()
    
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------
