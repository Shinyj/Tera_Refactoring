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
// RimLight
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string RimLight_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Teapot.3ds";

float4x4 WorldMatrix : World;
float4x4 ViewMatrix : View;
float4x4 ProjectionMatrix : Projection;


struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float3 Normal : NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 WorldPosition : TEXCOORD2;
};

VS_OUTPUT RimLight_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

  float4 worldPosition = mul(Input.Position, WorldMatrix);
  float4 viewPosition = mul(worldPosition, ViewMatrix);
  Output.Position = mul(viewPosition, ProjectionMatrix);
   
  Output.WorldPosition = worldPosition;
  Output.Normal = mul(Input.Normal, WorldMatrix);
  Output.TexCoord = Input.TexCoord;
  
   return( Output );   
}




texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D Texture0 = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct PS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 WorldPosition : TEXCOORD2;
};

float Rim_Start
<
   string UIName = "Rim_Start";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 0.00;
> = float( 0.65 );
float Rim_End
<
   string UIName = "Rim_End";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 1.00 );
float Rim_Multiplier
<
   string UIName = "Rim_Multiplier";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 1.00 );
float4 Rim_Color
<
   string UIName = "Rim_Color";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 1.00, 0.00, 0.00, 0.00 );
float3 LightDirection
<
   string UIName = "LightDirection";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 1.00, -1.00, 1.00 );
float3 LightColor
<
   string UIName = "LightColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.90, 0.90, 0.90 );
float3 AmbientColor
<
   string UIName = "AmbientColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.40, 0.40, 0.40 );
float4 ViewI : ViewPosition;
float  Alpha
<
   string UIName = "Alpha";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.00 );
float Offset
<
   string UIName = "Offset";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 0.00;
> = float( 0.20 );

float4 RimLight_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
  float3 color;
  color = tex2D(Texture0, Input.TexCoord);
  
  float3 CameraPosition = ViewI;
  float3 N = normalize(Input.Normal);
  float3 V = normalize(CameraPosition - Input.WorldPosition);
  float rim = smoothstep(Rim_Start,Rim_End,1-dot(N,V));
  
  float3 L = normalize(-LightDirection);
  float lightAmount = max(dot(N,L),0);
  float lighting = AmbientColor + lightAmount * LightColor;
  
  float4 f = float4(color,1) * lighting  + rim*Rim_Multiplier * Rim_Color;
  //f.w = 0;
  f.x += (0.2f - Offset);
  f.y += (0.2f - Offset);
  f.z += (0.2f - Offset);
  
  return float4(f.xyz, Alpha); 
}




//--------------------------------------------------------------//
// Technique Section for RimLight
//--------------------------------------------------------------//
technique RimLight
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 RimLight_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 RimLight_Pass_0_Pixel_Shader_ps_main();
   }

}

