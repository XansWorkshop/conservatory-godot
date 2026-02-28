# This script is a stupid hack.

# So for context, a change to this fork's engine involves using System.Numerics types for Vector2/3/4 among other things.
# A side effect of this change is that references to certain inner types, like Vector3.Axis, are invalidated.

# So this is the fix. It's fucking stupid. But it works.
# In a C# script I have a global using that assigns names to supplemental types.

# So...


import os
import os.path
import sys
from os import listdir
from os.path import isfile, join, abspath

base_path = sys.argv[1]

# lmfao
# stole shits from so
def inplace_change(filename, old_string, new_string):
    # Safely read the input filename using 'with'
    with open(filename, 'rt', encoding="utf-8") as f:
        s = f.read()
        if old_string not in s:
            return

    # Safely write the changed content, if found in the file
    with open(filename, 'wt', encoding="utf-8") as f:
        s = s.replace(old_string, new_string)
        f.write(s)


gencode = abspath(join(base_path, "modules", "mono", "glue", "GodotSharp", "GodotSharp", "Generated", "GodotObjects"))
print(gencode)
for f in listdir(gencode):
    if isfile(join(gencode, f)) and f.endswith(".cs"):
        # Actually awful.
        print("Patching " + join(gencode, f))
        inplace_change(join(gencode, f), "Vector2.Axis", "Vector2Axis")
        inplace_change(join(gencode, f), "Vector3.Axis", "Vector3Axis")
        inplace_change(join(gencode, f), "Vector4.Axis", "Vector4Axis")