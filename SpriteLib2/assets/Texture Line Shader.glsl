#version 330 core
out vData
{
	vec2 texcoord;
	vec3 color;	//make vec4 if I want transparancy, (could also just pass through an alpha float)
} frag;

layout(points) in;									//takes in a point
layout(line_strip, max_vertices = 2) out;	

//Everything comes into the cpu in view space rather than world space when raycasting!
//Could optimize this to make better use of the gpu by instancing the magnifier (could have multiple if I wanted some weird distortions),
//real hights, and yeah there's no actual need to send positions (other than a starting height) cause instancing will be done for every horizontal pixel.
//uniform mat4 modelView;	//don't need model view if working with just point, only rects and circles
uniform mat4 projection;

uniform vec3 posA;			//known point
uniform float wallHeight;	//how far away vertically from the known point
//uniform float fracValue;
uniform float U;
uniform vec3 color;
uniform float distance;

void main()
{	//I'm still not sure how this is working. Perhaps we're relying on rasterizer interpolation?
	//Pretty sure that's exactly what's happening. fract() maps 
	gl_Position = projection * vec4(posA.x, posA.y + wallHeight/2.0f, 0.0, 1.0);
	frag.texcoord = vec2(fract(U), 1.0f);	//sets the uv with v at the top of the line
	frag.color = color;						//colours upper half
	EmitVertex();
	
	gl_Position = projection * vec4(posA.x, posA.y - wallHeight/2.0f, 0.0, 1.0);
	frag.texcoord = vec2(fract(U), 0.0f);	//sets the uv with v at the bottom of the line
	frag.color = color;						//colours lower half
	EmitVertex();

	EndPrimitive();
}