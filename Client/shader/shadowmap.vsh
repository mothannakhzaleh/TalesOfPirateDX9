ps.1.1			//version 1.1
tex t0			//sample shadow map

add r1, v0, v1	//r1=diffuse+specular
mul r0, r1, t0	//r0=(diffuse+specular)*shadow map