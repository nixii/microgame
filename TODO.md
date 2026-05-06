
## To-Do list

TODO #5 - preperation for test platformer:
 * `[x]` Mouse delta detection
 * `[ ]` Collider gets scaled
 * `[ ]` Mouse click detection
 * `[ ]` Fix rendering flickers
 * * `[ ]` depth-buffer fix
 * * `[ ]` fix triangles being deleted if all 3 points outside screen
 * `[ ]` Closest hit collider only is counted
 * `[ ]` Design the scripting language

TODO #4 - various improvements:
 * `[x]` UI anchor
 * `[x]` UI text auto-wrapping
 * `[x]` UI tint
 * * `[x]` Stop it randomly going transparent for some reason
 * `[x]` Alpha for text working better than a firm cutoff
 * * `[x]` Alpha mix function
 * * `[x]` Alpha mix whenever changing any 
 * `[x]` Text can be colored (the entire thing)
 * `[x]` 3d box collider (doesn't rotate)
 * * `[x]` Detect 3d collision
 * * `[x]` Run on-collide functions
 * * `[x]` Get certain faces of the collider's position (for alignment)

TODO #3 - UI (in order of priority):
 * `[x]` UI vectors
 * `[x]` Custom UI layer
 * `[x]` UI transforms work (no rotation here)
 * `[x]` Image loading into 2d arrays & a resource
 * `[x]` Base UI objects
 * * `[x]` ui_container
 * * `[x]` ui_rect
 * * `[x]` ui_image
 * * `[x]` ui_button
 * `[x]` make the ui image scale based on element scale
 * `[x]` Load text from a small font file (just a png image cut up)
 * `[x]` More UI types
 * * `[x]` ui_text

TODO #2 (in order of priority):
 * `[x]` Improve performance of rendering meshes
 * `[x]` Modify the entity system to allow for parent/child relationships
 * `[x]` Apply translation, rotation, and scale down the tree
 * * `[x]` Translation done
 * * `[x]` Rotation done
 * * `[x]` Scale done
 * * `[x]` All these perform actions on the mesh too
 * `[x]` Load file meshes into a "resource", not into a mesh (literal meshes are still stored)
 * `[x]` Optimize 3d rendering by storing global matrices each frame

TODO #1 (in order of priority):
 * `[x]` Depth sort the triangles
 * `[x]` Meshes now use indices and vertices to save storage space
 * `[x]` Near clipping
 * * `[x]` 1 point out clipping
 * * `[x]` 2 point out clipping
 * * `[x]` 3 point out clipping
 * `[x]` *correct* back face culling
 * `[x]` Loading .obj files into a mesh