# PaNiON OpenGL [DEVELOPMENT CLOSED!]
A openGL Engine I'm making to make my own games and learn openGL
All the libraries `glfw3.lib` `opengl32.lib` are to be added
Also include the `glad.c` `stb_image.h` file in your source files folder

<h2>Hello Sqaure</h2>
Here's a sqaure that I created. 
Color of background: <b>0.1f, 0.1f, 0.12f, 1.0f</b>, 
Color of Square: <b>0.9f, 0.1f, 0.2f, 1.0f</b>
<img style="width: 40%;" src="https://cdn.discordapp.com/attachments/890224882086985728/915594323419086908/unknown.png" alt="hello-quare"></img>


<h2>3D Rotating CUBE!</h2>
Here's a 3D Fully textured cube
and here are it's vertices and indices with textCords:

```c++
float vertices[] = {
	0.5f, 0.5f, 0.5f,			1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,			0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,			1.0f, 0.0f,			//front face

	-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,			//top face

	0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,			0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,			1.0f, 0.0f,			//back face

	-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,			0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,			1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,			1.0f, 0.0f,			//bottom face	

	0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 1.0f,			//right face

	-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,			0.0f, 1.0f			//left face
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20
};
```
OutPut: <br>
<img style="width: 30%;" src="https://cdn.discordapp.com/attachments/890224882086985728/915831022661304360/unknown.png">

<hr>
<h2>Using VSCODE and GCC Compiler</h2>
first of all download the GLFW x32 pre compiled binaries and add the `glfw3.dll` to your `build` folder. And also add `libglfw3dll.a` to your `libs` folder, now include `GLFW, glad` add them in your include folder.

now to compile enter the following command:
`g++ -IInclude -LLibs .\source.cpp -o Build\Opengl.exe -lopengl32 -lgfw3dll`
