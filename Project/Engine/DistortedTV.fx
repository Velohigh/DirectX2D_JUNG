#ifndef _DISTORTEDTV
#define _DISTORTEDTV

#include "value.fx"
#include "func.fx"

#define iChannel0 g_tex_0
#define iChannel1 g_tex_1
#define iTime g_AccTime

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

float noise(float2 p)
{
    float s = g_tex_1.Sample(g_sam_1, float2(1.0, 2.0 * cos(iTime)) * iTime * 8.0 + p * 1.0).x;
    s *= s;
    return s;
}

float onOff(float a, float b, float c)
{
    return step(c, sin(iTime + a * cos(iTime * b)));
}

float ramp(float y, float start, float end)
{
    float inside = step(start, y) - step(end, y);
    float fact = (y - start) / (end - start) * inside;
    return (1.0 - fact) * inside;
	
}

float stripes(float2 uv)
{
    float noi = noise(uv * float2(0.5, 1.0) + float2(1.0, 3.0));
    return ramp(fmod(uv.y * 4.0 + iTime / 2.0 + sin(iTime + sin(iTime * 0.63)), 1.0), 0.5, 0.6) * noi;
}

float3 getVideo(float2 uv)
{
    float2 look = uv;
    float window = 1.0 / (1.0 + 20.0 * (look.y - fmod(iTime / 4.0, 1.0)) * (look.y - fmod(iTime / 4.0, 1.0)));
    look.x = look.x + sin(look.y * 10.0 + iTime) / 50.0 * onOff(4.0, 4.0, 0.3) * (1.0 + cos(iTime * 80.0)) * window;
    float vShift = 0.4 * onOff(2.0, 3.0, 0.9) * (sin(iTime) * sin(iTime * 20.0) +
										 (0.5 + 0.1 * sin(iTime * 200.0) * cos(iTime)));
    look.y = fmod(look.y + vShift, 1.0);
    float3 video = float3(g_tex_0.Sample(g_sam_0, look).xyz);
    return video;
}

float2 screenDistort(float2 uv)
{
    uv -= float2(0.5, 0.5);
    uv = uv * 1.2 * (1.0 / 1.2 + 2.0 * uv.x * uv.x * uv.y * uv.y);
    uv += float2(0.5, 0.5);
    return uv;
}

// ============================
// VCRDistortion Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Noise Texture
// ============================

VS_OUT VS_VCRDistortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}


float4 PS_VCRDistortion(VS_OUT _in) : SV_Target
{
    float2 uv = _in.vPosition.xy / g_Resolution.xy;
    uv = screenDistort(uv);
    float3 video = getVideo(uv);
    float vigAmt = 3.0 + 0.3 * sin(iTime + 5.0 * cos(iTime * 5.0));
    float vignette = (1.0 - vigAmt * (uv.y - 0.5) * (uv.y - 0.5)) * (1.0 - vigAmt * (uv.x - 0.5) * (uv.x - 0.5));
	
    video += stripes(uv);
    video += noise(uv * 2.0) / 2.0;
    video *= vignette;
    video *= (12.0 + fmod(uv.y * 30.0 + iTime, 1.0)) / 13.0;
	
    float4 Color = float4(video, 1.0);
    return Color;
}


// ============================
// Texture twistery Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Noise Texture
// ============================

float2 getDistortion(float2 uv, float d, float t)
{
    uv.x += cos(d) + t * 0.9;
    uv.y += sin(d + t * 0.75);
    return uv;
}

float4 getDistortedTexture(Texture2D iChannel, float2 uv)
{
    float4 rgb = iChannel.Sample(g_sam_1, uv);
    return rgb;
}

VS_OUT VS_TexTwistery(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}


float4 PS_TexTwistery(VS_OUT _in) : SV_Target
{
    float2 uv = _in.vPosition.xy / g_Resolution.xy;
    float t = iTime;
    float2 mid = float2(0.5, 0.5);
    float2 focus = g_MousePos.xy / g_Resolution.xy;
    float d1 = distance(focus + sin(t * 0.25) * 0.5, uv);
    float d2 = distance(focus + cos(t), uv);
    
    float4 rgb = (getDistortedTexture(g_tex_0, getDistortion(uv, d1, t)) + getDistortedTexture(g_tex_1, getDistortion(uv, -d2, t))) * 0.5;
    rgb.r /= d2;
    rgb.g += -0.5 + d1;
    rgb.b = -0.5 + (d1 + d2) / 2.0;
    float4 Color = rgb;
    return Color;
}

//    float2 vUV = _in.vPosition.xy / g_Resolution;
	
//	// Noise Texture 가 세팅이 되어 있다면
//    if (g_btex_1)
//    {
//        float2 vNoiseUV = float2(_in.vUV.x - (g_AccTime * 0.2f), _in.vUV.y);
//        float4 vNoise = g_tex_1.Sample(g_sam_0, vNoiseUV);

//        vNoise = (vNoise - 0.5f) * 0.02f;

//        vUV += vNoise.r;
//    }

//    float4 vColor = g_tex_0.Sample(g_sam_0, vUV);

//    return vColor;

#endif
