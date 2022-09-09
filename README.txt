Camera:
Use the mouse and drag, to control the pitch and yaw of the camera, and use the mouse to translate the camera forwards and backwards.
Sliders on the side can also be used to control these parameters. 
Phong:
Use the slider labeled "Mesh Colour" to change the colour of the central teapot, and use the slider labeled "Specular Colour" to change the specular colour
of all the teapots. Use the slider labeled "Specular Strength" to control how shiny the specularity is for all the teapots. 
Use the slider labeled "Ambient strength" to control the strength of the ambience. 
There are also sliders that can be used to change the light colour. 
Instancing:
Notice that there are 100 teapots in the scene created using instancing. They each have different colours, rotations, translations and scales.
Textures:
Use the checkbox named "Textured" to toggle on or off the teapots using a texture or being a single colour. 
The texture can be changed on line 51 of application.cpp
Bounding Boxes:
Use the checkbox provided to toggle bounding boxes on or off. 


Additonal Information:
Similar to my previous assignment the application will always crash when minimized. Even just from the base code provided for this assignment. 
Again, visual studios doesnt always run the application properly and sometimes will just show a blank window. Running the executable from:
Work -> out -> build -> x64-Debug -> bin and running "base", is much more reliable and consistent. 

Using the ToonStrength slider, A ToonShader effect can be created when the slider is negative. When the shader is 1 then a different affect will occur
specular colour can be used with this effect to control how light affects the teapots. 

The translationOffset sliders can be used to control the offset of the teapots/bounding boxes, in each axis independently. 
