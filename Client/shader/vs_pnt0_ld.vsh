vs.1.1


//compute position
m4x4 oPos, v0, c1

// normalize normals
mov r5, v3;
dp3 r5.w, r5, r5;
rsq r5.w, r5.w;
mul r5, r5, r5.w;


; Do the lighting calculation
mov r0, -c5
dp3 r5.x, r5, r0      ; normal dot light
lit r5, r5
mul r0, r5.y, c7      ; Multiply with diffuse
add r0, r0, c6        ; Add in ambient
min oD0, r0, c0.x     ; clamp if > 1
mov oD1, c0.zzzz      ; output specular

; uv texture coordinate
mov oT0, v7

