vs.1.1

dcl_position v0
dcl_blendindices v2
dcl_normal v3
dcl_texcoord0 v7

mul r1.xyzw, v2.zyxw, c0.wwww
mov a0.x, r1.xxxx
m4x3 r4.xyz, v0.xyzw, c[a0.x + 21].xyzw
m3x3 r5.xyz, v3.xyzw, c[a0.x + 21].xyzw
mov r4.w, c0.xxxx
m4x4 oPos.xyzw, r4.xyzw, c1.xyzw
dp3 r5.w, r5.xyzw, r5.xyzw
rsq r5.w, r5.wwww
mul r5.xyzw, r5.xyzw, r5.wwww
dp3 r1.x, r5.xyzw, c5.xyzw
lit r1.xyzw, r1.xyzw
mul r0.xyzw, r1.yyyy, c7.xyzw
add r0.xyzw, r0.xyzw, c6.xyzw
min oD0.xyzw, r0.xyzw, c0.xxxx
mov oD1.xyzw, c0.zzzz
mov oT0.xyzw, v7.xyzw
