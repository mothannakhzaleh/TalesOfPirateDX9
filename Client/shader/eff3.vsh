//c0 - c3 //world
//c4 - c7 //view * proj
//c8	  //alpha value
//c9 - c12//vertex uv
vs.1.1

m4x4 r0, v0, c0
m4x4 oPos,r0,c4    
 // Transform position
//mov oPos, v0
mov oD0,  c8
mov a0.x, v2
mov oT0.xy,c[a0.x + 9].xy

