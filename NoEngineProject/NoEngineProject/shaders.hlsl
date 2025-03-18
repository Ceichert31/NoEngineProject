/* vertex attributes go here to input to the vertex shader */
struct VertexInput
{
	float4 localPosition : POSITION;
	float4 color : COLOR;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct VertexOutput
{
	float4 screenPosition : SV_POSITION; // required output of VS
	float4 color : COLOR;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output;
	
	output.screenPosition = input.localPosition;
	output.color = input.color;
	
	return output;
}

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	return input.color; // must return an RGBA colour
}