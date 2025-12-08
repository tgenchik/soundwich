#include "audio_output/audio_output.hpp"
#include <iostream>

using namespace soundwich;

void soundwich::process(void* core_ptr)
{
    PipeWireCore* core = static_cast<PipeWireCore*>(core_ptr);
    pw_buffer* b;
    spa_buffer* buf;
    int i, c, n_frames, stride;
    float* dst;

    if ((b = pw_stream_dequeue_buffer(core->stream)) == NULL)
    {
        pw_log_warn("out of buffers: %m");
        return;
    }

    buf = b->buffer;
    if ((dst = static_cast<float*>(buf->datas[0].data)) == NULL)
        return;

    stride = sizeof(float) * core->channels;
    n_frames = buf->datas[0].maxsize / stride;
    if (b->requested)
        n_frames = SPA_MIN(b->requested, n_frames);

    memset(dst, 0, n_frames * sizeof(float) * core->channels);

    float* prev_dst;
    PipeWireOutput* it;
    for (int O = 0; O < core->items.size(); O++)
    {
        std::lock_guard<std::mutex> lock(core->muts[O]);
        it = core->items[O];

        if (it == nullptr) continue;
        if (it->state != playing)
            continue;

        prev_dst = dst;
        for (i = 0; i < n_frames; i++)
        {
            it->timer += 1.0 / core->rate;
            for (int j = 0; j < core->channels; j++)
                *dst++ += it->getNext() * it->volume;
        }

        dst = prev_dst;
        if (it->dieOnEnd_flag && it->data.empty())
        {
            core->muts[i].unlock();
            delete it;
        }
    }

    buf->datas[0].chunk->offset = 0;
    buf->datas[0].chunk->stride = stride;
    buf->datas[0].chunk->size = n_frames * stride;

    pw_stream_queue_buffer(core->stream, b);
}
