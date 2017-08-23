Texture2D gTextureRT : register(t4);

RWTexture2D<float4> gOutput : register(u0);

#define THREAD_GROUD_COUNT 256

[numthreads(THREAD_GROUD_COUNT, 1, 1)]
void CS_HORIZONTAL_INTERACE(int3 groupID : SV_GroupID,
							int3 groudThreadID : SV_GroupThreadID,
							int3 dispatchThreadID : SV_DispatchThreadID)
{

}