
vs.1.1

;------------------------------------------------------------------------------
; v0 = position
; v1 = blend weights
; v2 = blend indices
; v3 = normal
; v7 = texture coordinates
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; r0.w = Last blend weight
; r1 = Blend indices
; r2 = Temp position
; r3 = Temp normal
; r4 = Blended position in camera space
; r5 = Blended normal in camera space
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Constants specified by the app;
;
; c21-c95 = world-view matrix palette
; c7	  = diffuse * light.diffuse
; c6	  = ambient color
; c1-c4   = projection matrix
; c5	  = light direction
; c0	  = {1, power, 0, 1020.01};
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; oPos	  = Output position
; oD0	  = Diffuse
; oD1	  = Specular
; oT0	  = texture coordinates
;------------------------------------------------------------------------------

;dcl_position v0;
;dcl_blendweight v1;
;dcl_blendindices v2;
;dcl_normal v3;
;dcl_texcoord0 v7;

// Compensate for lack of UBYTE4 on Geforce3
mul r1,v2.zyxw,c0.wwww
//mul r1,v2,c0.wwww


//first compute the last blending weight
dp3 r0.w,v1.xyz,c0.xzz; 
add r0.w,-r0.w,c0.x

//Set 1
mov a0.x,r1.x
m4x3 r4.xyz,v0,c[a0.x + 21];
m3x3 r5.xyz,v3,c[a0.x + 21]; 

//blend them
mul r4.xyz,r4.xyz,v1.xxxx
mul r5.xyz,r5.xyz,v1.xxxx

//Set 2
mov a0.x,r1.y
m4x3 r2.xyz,v0,c[a0.x + 21];
m3x3 r3.xyz,v3,c[a0.x + 21];

//add them in
mad r4.xyz,r2.xyz,r0.wwww,r4;
mad r5.xyz,r3.xyz,r0.wwww,r5;

//compute position
mov r4.w,c0.x
m4x4 oPos,r4,c1

// normalize normals
dp3 r5.w, r5, r5;
rsq r5.w, r5.w;
mul r5, r5, r5.w;

; Do the lighting calculation
dp3 r1.x, r5, c5      ; normal dot light
lit r1, r1
mul r0, r1.y, c7      ; Multiply with diffuse
add r0, r0, c6        ; Add in ambient
min oD0, r0, c0.x     ; clamp if > 1
mov oD1, c0.zzzz      ; output specular

; Copy texture coordinate
mov oT0, v7

mov oT1, v7
mov oT3, v7

mov r4, v7
mov r4.z, c0.x
m4x3 r4, r4, c16
mov oT2, r4
