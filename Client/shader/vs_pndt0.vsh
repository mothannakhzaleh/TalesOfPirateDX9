vs.1.1


//compute position
m4x4 oPos, v0, c1


//Do the lighting calculation
mul r0, v5, c7		  ; modulate diffuse 
add r0, r0, c6        ; Add in ambient
min oD0, r0, c0.x     ; clamp if > 1
mov oD1, c0.zzzz      ; output specular

; uv texture coordinate
mov oT0, v7

