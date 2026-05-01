
## To-Do list

TODO #3 - UI (in order of priority):
 * `[x]` UI vectors
 * `[x]` Custom UI layer
 * `[x]` UI transforms work (no rotation here)
 * `[ ]` Image loading into 2d arrays & a resource
 * `[ ]` Base UI objects
 * * `[x]` ui_container
 * * `[x]` ui_rect
 * * `[ ]` ui_image
 * * `[ ]` ui_image_button
 * `[ ]` Load text from a small font file (just a png image cut up)
 * `[ ]` More UI types
 * * `[ ]` ui_text
 * * `[ ]` ui_text_button

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