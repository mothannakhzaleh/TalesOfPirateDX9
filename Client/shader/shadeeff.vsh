//c0 - c3 //world
//c4 - c7 //view * proj
//c8	  //alpha value
//c9 - c12//vertex and uv
vs.1.1

//def c90, 1.0f,1.0f,1.0f,1.0f

mov a0.x, v8.y
mov r0.xyzw, c[a0.x].xyzw
//mul r0, r0.xyzw, c90

//m4x4 r0,r0,c0

m4x4 oPos,r0,c4

mov oD0,  c8

mov a0.x, v8.x
mov oT0.xy,c[a0.x].xy