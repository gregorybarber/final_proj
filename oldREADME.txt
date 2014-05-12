Yujin Ariza
yra2101
Graphics pa-3

Description
This project demonstrates a few lighting and surface shaders on two
teapots.

Instructions
Use the right mouse button to rotate around the scene. The scene starts
in Gouraud Shading mode, with two lights on the scene. You can mix and 
match 5 different shading functions, each assigned to the keys 1-5. They 
are as follows:

1 - Switch to Gouraud Shading. If already in Gouraud mode, switch to
    flat shading.
2 - Switch to Blinn-Phong Shading. If already in Blinn-Phong mode, switch
    to flat shading.
3 - Switch to a checkerboard texture. If already in Checkerboard mode,
    switch to default color (red and green teapots).
4 - Switch to a color texture, with the source file "smooth.png". If
    already in color texture mode, switch to default color.
5 - Toggle Normal Mapping. Note that this requires Blinn-Phong shading,
    as Gouraud is vertex-based and does not calculate normals in the 
    fragment stage. The source file used is "drops.png", which gives the
    impression of a teapot covered in water droplets.

Presing 'r' takes a screenshot of the window, and ESC quits the 
application.

Implementation Details
The Image loading library SOIL was used for this project, for loading .png
files. More information and documentation about SOIL can be found at:
http://www.lonesock.net/soil.html

Rather than switch between multiple shaders, I used one big shader with all
the features included, with uniform variables passed in from the 
application to toggle the individual elements. To demonstrate the 
functionality of multiple lights, there are two lights in the scene,
with slightly different positions. 
There are several textures included in the directory textures/ ; these are
all interesting files I found from the internet and/or composited using
Adobe Photoshop. You can use them by editing the variables normal_file
and color_file near the top of main.cpp. 

Sample pictures of the different effects achieved can be seen in the 
directory pics/ .

References
I consulted a few online sources to build this project. They are 
listed as follows:
http://en.wikibooks.org/wiki/GLSL_Programming/GLUT
http://www.lighthouse3d.com/tutorials/glsl-tutorial/

