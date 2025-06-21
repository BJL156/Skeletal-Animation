# Skeletal Animation Renderer
This is a simple skeletal animation system made using OpenGL, C++, tinygltf, and GLM. It supports both static and animated 3D models from `.gltf` files.

# 3D Model Credits
[Animated Tyrannosaurus Rex Dinosaur Running Loop](https://sketchfab.com/3d-models/animated-tyrannosaurus-rex-dinosaur-running-loop-38007d947ae74dea83988cb0b08ee053) by [LasquetiSpice](https://sketchfab.com/LasquetiSpice).

The license to use this model can be found within: [Skeletal-Animation/OpenGL 3D Testing/resources/models/license.txt](https://github.com/BJL156/Skeletal-Animation/blob/main/OpenGL%203D%20Testing/resources/models/license.txt)

# Features
- Load and render models with or without animations.
- GLTF file loading using tinygltf.
- Basic "free fly" camera system.
- Ability to switch animations using the up and down arrow keys.

# Why isn't my model loading correctly?
If your model isn't loading correctly, one fix is to upload it to Blender to ensure if the model is the issue. After verifying that the model is working in Blender as expected, then try to export it and use the exported version instead.
