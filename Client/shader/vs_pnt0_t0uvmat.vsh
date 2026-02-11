vs.1.1


//compute position
m4x4 oPos, v0, c1


//Do the lighting calculation
mov r0, c7		  ; modulate diffuse 
add r0, r0, c6        ; Add in ambient
min oD0, r0, c0.x     ; clamp if > 1
mov oD1, c0.zzzz      ; output specular

; uv texture coordinate
mov r0, v7
mov r0.z, c0.x
dp3 r1.x, r0, c8
dp3 r1.y, r0, c9
mov oT0.xy, r1 
