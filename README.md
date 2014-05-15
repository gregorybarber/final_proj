#Final Project
Yujin Ariza / Gregory Barber  
Professor Changxi Zheng  
Computer Graphics (W4160)  
May 14, 2014


##I. Introduction:

We have a modelled a rainy/snowy cityscape. The city rises from the ground around a park, which includes recursively/randomly generated trees. The rain falls, collecting in growing, reflective puddles, and gradually turns to snow as autumn foliage appears and eventually falls away. 

We wrote individual shaders for each object and particle effect, aiming to produce realistic models, environment, and precipitation. We have implemented a number of features in addition to those already mentioned, including animations, textures, fog, and reflections, which are described in more detail below:

To run:
	cmake .
	make 
	./final

##II. Features

- Buildings:
	- Animated to rise out of the ground at varying rates
	- Textured with building windows
	- Roofs shaded to produce reflective puddles; other faces have their own shader
	- Additionally shader produce fogs effect (increasing opacity w/ distance from camera)
	- Variety of sizes to produce realistic city effect

- Trees:
	- Recursively and randomly generated branches to produce realistic model
	- Shader produces transparent effect (so that branches can be seen through leaves)
	- Foliage generated via recursive spheres
- Changing seasons: foliage gradually shifts in color from green to red
- once red, the transparency increases to transition into winter

- Particle Effects (Rain and Snow)
	- Generated using randomized particle system with particle path tracing
- rain = gl_lines and snow = deformed spheres
	- Rain and snow each have a shader
	- Changing seasons: Starts as rain, snow mixes in, ends all snow 
- change occurs in parallel with the changes to the tree foliage 

- Environment:
	- Ground shaded to produce reflective puddles and textured with concrete
		- these puddles increase in size as the length of the rainy period increases
	- Skybox used to produce sky color

##References
We used the SOIL image library in loading in textures and cubemaps. More information about SOIL can be found at: [http://www.lonesock.net/soil.html](http://www.lonesock.net/soil.html)