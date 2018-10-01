//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// DeathShader
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string DeathShader_Pass_0_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 matViewProjection : ViewProjection;
float4x4 matWorld : World;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord: TEXCOORD0;
};

VS_OUTPUT DeathShader_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matViewProjection );

   Output.TexCoord = Input.TexCoord;
   
   return( Output );
   
}




float Offset
<
   string UIName = "Offset";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 0.00;
> = float( 1.13 );

struct PS_INPUT
{
   float2 TexCoord: TEXCOORD0;
};

texture Earth_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D Texture0 = sampler_state
{
   Texture = (Earth_Tex);
};

float4 DeathShader_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   float4 albedo = tex2D(Texture0, Input.TexCoord);
   clip(albedo.r < Offset ? -1:1); 
   return albedo.rgba;
   
}




//--------------------------------------------------------------//
// Technique Section for DeathShader
//--------------------------------------------------------------//
technique DeathShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 DeathShader_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 DeathShader_Pass_0_Pixel_Shader_ps_main();
   }

}

