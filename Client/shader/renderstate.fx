// Technique 't0' will render the sample scene in one pass. 

technique t0
{
    pass p0
    {
		PixelShader  = NULL;
		
		ZEnable			 = TRUE;
		ZWriteEnable	 = TRUE;
		
		VertexBlend      = Disable;
		
		StencilEnable	 = FALSE;
		DitherEnable	 = TRUE;
		FogEnable		 = FALSE;
		SpecularEnable	 = FALSE;
		Zbias			 = 1;
		EdgeAntialias	 = TRUE;
		
		CullMode		 = None;
		Lighting		 = TRUE;
		
		AddressU[0]=WRAP;
        AddressV[0]=WRAP;
        
        AlphaTestEnable  = FALSE;
        
		AlphaBlendEnable = FALSE;
		//SrcBlend		 = SrcAlpha;
		//DestBlend		 = DestAlpha;
        
        ColorOp[0] = Modulate;
        ColorArg1[0] = Texture;
        ColorArg2[0] = Diffuse ;
 
        AlphaOp[0] = Modulate;
        AlphaArg1[0] = Texture;
        AlphaArg2[0] = Diffuse;
		        		
        ColorOp[1] = Disable;
        AlphaOp[1] = Disable;
    }

}

