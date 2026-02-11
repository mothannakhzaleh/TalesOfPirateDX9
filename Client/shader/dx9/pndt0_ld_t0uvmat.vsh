vs.1.1

dcl_position v0
dcl_normal v3
dcl_color v5
dcl_texcoord0 v7

m4x4 oPos.xyzw, v0.xyzw, c1.xyzw
mov r5.xyzw, v3.xyzw
dp3 r5.w, r5.xyzw, r5.xyzw
rsq r5.w, r5.wwww
mul r5.xyzw, r5.xyzw, r5.wwww
mov r0.xyzw, -c5.xyzw
dp3 r5.x, r5.xyzw, r0.xyzw
lit r5.xyzw, r5.xyzw
mul r0.xyzw, v5.xyzw, c7.xyzw
mul r0.xyzw, r5.yyyy, r0.xyzw
add r0.xyzw, r0.xyzw, c6.xyzw
min oD0.xyzw, r0.xyzw, c0.xxxx
mov oD1.xyzw, c0.zzzz
mov r0.xyzw, v7.xyzw
mov r0.z, c0.xxxx
dp3 r1.x, r0.xyzw, c8.xyzw
dp3 r1.y, r0.xyzw, c9.xyzw
mov oT0.xy, r1.xyzw
