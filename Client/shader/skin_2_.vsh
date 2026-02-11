
vs.1.1

;------------------------------------------------------------------------------
; v0 = position
; v1 = blend weights
; v2 = blend indices
; v3 = normal
; v4 = texture coordinates
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
; c12-c95 = world-view matrix palette
; c8	  = diffuse * light.diffuse
; c7	  = ambient color
; c6	  = {1.0f, fFogEnd, 1.0f/fDistance, 0.0f}
; c2-c5   = projection matrix
; c1	  = {1.0f, Power, fProgress, 765.01f} 
; c0	  = light direction
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; oPos	  = Output position
; oD0	  = Diffuse
; oD1	  = Specular
; oT0	  = texture coordinates
;------------------------------------------------------------------------------


// Compensate for lack of UBYTE4 on Geforce3
mul r1,v2.zyxw,c1.wwww
//mul r1,v2,c1.wwww


//first compute the last blending weight
dp3 r0.w,v1.xyz,c1.xzz; 
add r0.w,-r0.w,c1.x

//Set 1
max r1.x, r1.x, c6.w
mov a0.x,r1.x
m4x3 r4,v0,c[a0.x + 15];
m3x3 r5,v3,c[a0.x + 15]; 

//blend them
mul r4,r4,v1.xxxx
mul r5,r5,v1.xxxx

//Set 2
max r1.y, r1.y, c6.w
mov a0.x,r1.y
m4x3 r2,v0,c[a0.x + 15];
m3x3 r3,v3,c[a0.x + 15];

//add them in
mad r4,r2,r0.wwww,r4;
mad r5,r3,r0.wwww,r5;

//compute position
mov r4.w,c1.x
m4x4 oPos,r4,c2

// normalize normals
dp3 r5.w, r5, r5;
rsq r5.w, r5.w;
mul r5, r5, r5.w;

; Do the lighting calculation
dp3 r1.x, r5, c0      ; normal dot light
max r1.x, r1.x, c1.y     ; clamp if < 0
;lit r1, r1
mul r0, r1.x, c8      ; Multiply with diffuse
add r0, r0, c7        ; Add in ambient
min oD0, r0, c1.x     ; clamp if > 1
mov oD0.w, c10.y
;mov oD1, c1.zzzz      ; output specular
;mov oD1, c1.yyyy      ; output specular

;fog
;mul r4.z, r4.z, c1.z
;sub r4.z, c1.x, r4.z
;mov oFog, r4.z

; compute fog factor f = (fog_end - dist)*(1/(fog_end-fog_start))
add r0.x, -r4.z, c6.y
mul r0.x, r0.x, c6.z
max r0.x, r0.x, c6.w       ; clamp fog to > 0.0
min oFog.x, r0.x, c6.x     ; clamp fog to < 1.0




;mov r4, v4
;add r4.x, r5.x, c1.z
;add r4.y, r4.y, c1.z

add r5.y, r5.y, c1.z

; Copy texture coordinate
mov oT0, v4
mov oT1, r5

