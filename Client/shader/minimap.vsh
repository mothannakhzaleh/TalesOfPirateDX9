//c0	  //alpha value
vs.1.1

mov a0.x, v1.x

mov r0.xyzw,   c[a0.x + 9].xyzw

//add r0, v0, r0.xyzw

m4x4  r4, r0, c0

m4x4  oPos, r4, c4

mov oD0,  c8

mov oT0.xy,  v7.xy
