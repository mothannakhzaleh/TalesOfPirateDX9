vs.1.1

dcl_position v0
dcl_color v5
dcl_texcoord0 v7

m4x4 oPos.xyzw, v0.xyzw, c1.xyzw
mul r0.xyzw, v5.xyzw, c7.xyzw
add r0.xyzw, r0.xyzw, c6.xyzw
min oD0.xyzw, r0.xyzw, c0.xxxx
mov oD1.xyzw, c0.zzzz
mov oT0.xyzw, v7.xyzw
