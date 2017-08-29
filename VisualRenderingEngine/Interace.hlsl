SamplerState gBasicSampler : register(s0);
Texture2D gTextureRT : register(t4);

RWTexture2D<float4> gOutput : register(u0);

extern uint gWindowWidth;
extern uint gWindowHeight;

#define THREAD_GROUD_COUNT 256

[numthreads(THREAD_GROUD_COUNT, 1, 1)]
void CS_VERTICAL_INTERACE(int3 groupID : SV_GroupID,
						  int3 groudThreadID : SV_GroupThreadID,
						  int3 dispatchThreadID : SV_DispatchThreadID)
{
    gOutput[dispatchThreadID.xy] = gTextureRT.SampleLevel(gBasicSampler, int2(dispatchThreadID.x, dispatchThreadID.y), 0);
    //gOutput[dispatchThreadID.xy] = float4(1.0f, 1.0f, 0.0f, 1.0f);
}