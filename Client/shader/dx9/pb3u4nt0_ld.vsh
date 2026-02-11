vs.1.1

dcl_position v0
dcl_blendweight v1
dcl_blendindices v2
dcl_normal v3
dcl_texcoord0 v7

mul r1.xyzw, v2.zyxw, c0.wwww
dp3 r0.w, v1.xyzz, c0.xxxx
add r0.w, -r0.wwww, c0.xxxx
mov a0.x, r1.xxxx
m4x3 r4.xyz, v0.xyzw, c[a0.x + 21].xyzw
m3x3 r5.xyz, v3.xyzw, c[a0.x + 21].xyzw
mul r4.xyz, r4.xyzz, v1.xxxx
mul r5.xyz, r5.xyzz, v1.xxxx
mov a0.x, r1.yyyy
m4x3 r2.xyz, v0.xyzw, c[a0.x + 21].xyzw
m3x3 r3.xyz, v3.xyzw, c[a0.x + 21].xyzw
mad r4.xyz, r2.xyzz, v1.yyyy, r4.xyzw
mad r5.xyz, r3.xyzz, v1.yyyy, r5.xyzw
mov a0.x, r1.zzzz
m4x3 r2.xyz, v0.xyzw, c[a0.x + 21].xyzw
m3x3 r3.xyz, v3.xyzw, c[a0.x + 21].xyzw
mad r4.xyz, r2.xyzz, v1.zzzz, r4.xyzw
mad r5.xyz, r3.xyzz, v1.zzzz, r5.xyzw
mov a0.x, r1.wwww
m4x3 r2.xyz, v0.xyzw, c[a0.x + 21].xyzw
m3x3 r3.xyz, v3.xyzw, c[a0.x + 21].xyzw
mad r4.xyz, r2.xyzz, r0.wwww, r4.xyzw
mad r5.xyz, r3.xyzz, r0.wwww, r5.xyzw
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
