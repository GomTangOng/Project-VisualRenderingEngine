SamplerState gBasicSampler : register(s0);
Texture2D gTextureRT : register(t4);

RWTexture2D<float4> gOutput : register(u0);   // why add unorm...?

extern uint gWindowWidth;
extern uint gWindowHeight;

#define THREAD_COUNT 16

[numthreads(THREAD_COUNT, THREAD_COUNT, 1)]
void CS_VERTICAL_INTERACE(int3 groupID : SV_GroupID,
						  int3 groudThreadID : SV_GroupThreadID,
						  int3 dispatchThreadID : SV_DispatchThreadID)
{
    gOutput[dispatchThreadID.xy] = gTextureRT[int2(dispatchThreadID.x, dispatchThreadID.y)];
    //gOutput[dispatchThreadID.xy] = gTextureRT.SampleLevel(gBasicSampler, int2(dispatchThreadID.x, dispatchThreadID.y), 0);   
}