//c0 - c3 //world
//c4 - c7 //view * proj
//c8	  //alpha value
//c9 - c12//vertex uv
vs.1.0

m4x4 r0, v0, c0
m4x4 oPos,r0,c4     // Transform position

mov oD0, c8
mov oT0.xy,v7.xy

