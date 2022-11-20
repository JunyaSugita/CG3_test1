#include "GrassShaderHeader.hlsli"

//�l�p�`�̒��_
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] = {
	float4(-0.5f,0.0f,0,0),
	float4(-0.5f, 1.0f,0,0),
	float4(0.5f,0.0f,0,0),
	float4(0.5f, 1.0f,0,0)
};

static const float2 uv_array[vnum] = {
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	for (uint i = 0; i < vnum; i++) {
		float4 offset = mul(matBillboard, offset_array[i]);
		//���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].pos + offset;
		//�r���[�A�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}