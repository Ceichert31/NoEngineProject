/* vertex attributes go here to input to the vertex shader */
struct VertexInput
{
	float3 localPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct VertexOutput
{
	float4 screenPosition : SV_POSITION; // required output of VS
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output = (VertexOutput) 0; // zero the memory first
	output.screenPosition = float4(input.localPosition, 1.0);
	return output;
}

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	return float4(1.0, 0.0, 1.0, 1.0); // must return an RGBA colour
}